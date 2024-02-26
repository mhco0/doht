// Doth Library (C) by Marcos Oliveira
#include <doht/shc/shc.h>

#include <cstddef>
#include <fstream>
#include <functional>
#include <iterator>
#include <map>
#include <memory>
#include <optional>
#include <queue>
#include <stack>
#include <string>
#include <vector>

namespace doht::shc {

namespace {
struct Node {
  std::optional<char> symbol;
  size_t frequency;
};

class CodingTree {
 public:
  void ClearTree() {
    weights_.clear();
    code_map_.clear();
  }

  void SetTree(const std::vector<Node>& weights,
               const std::map<char, std::vector<bool>>& code_map) {
    weights_ = weights;
    code_map_ = code_map;
  }

  void BuildTree(const std::map<char, int32_t>& frequencies) {
    if (weights_.size() || code_map_.size()) {
      ClearTree();
    }

    struct HeapNode : Node {
      HeapNode(const std::optional<char>& symbol, size_t freq) {
        this->symbol = symbol;
        this->frequency = freq;
      }

      std::shared_ptr<HeapNode> left{};
      std::shared_ptr<HeapNode> right{};

      bool operator>(const HeapNode& rhs) {
        return this->frequency > rhs.frequency;
      }
    };

    class SharedHeapNodeFunctor {
     public:
      bool operator()(const std::shared_ptr<HeapNode>& lhs,
                      const std::shared_ptr<HeapNode>& rhs) {
        if (!lhs.get()) {
          return false;
        }

        if (!rhs.get()) {
          return true;
        }

        return *lhs > *rhs;
      }
    };

    auto frequency_to_node =
        [](const std::pair<char, int32_t>& entry) -> std::shared_ptr<HeapNode> {
      return std::make_shared<HeapNode>(std::make_optional(entry.first),
                                        static_cast<size_t>(entry.second));
    };

    std::vector<std::shared_ptr<HeapNode>> leafs{};
    leafs.reserve(frequencies.size());
    for (auto& it : frequencies) {
      leafs.emplace_back(frequency_to_node(it));
    }

    std::priority_queue<std::shared_ptr<HeapNode>,
                        std::vector<std::shared_ptr<HeapNode>>,
                        SharedHeapNodeFunctor>
        min_heap(leafs.begin(), leafs.end());

    while (min_heap.size() > 1) {
      auto left_child = min_heap.top();
      min_heap.pop();
      auto right_child = min_heap.top();
      min_heap.pop();

      auto new_node = std::make_shared<HeapNode>(
          std::nullopt, left_child->frequency + right_child->frequency);
      new_node->left = left_child;
      new_node->right = right_child;

      min_heap.push(new_node);
    }

    auto tree_to_vector =
        [this](const std::shared_ptr<HeapNode>& root) -> void {
      std::stack<std::pair<size_t, std::shared_ptr<HeapNode>>> to_visit{};

      to_visit.push({0, root});

      while (!to_visit.empty()) {
        auto [index, node_visited] = to_visit.top();
        to_visit.pop();

        if (index >= weights_.size()) {
          weights_.resize((weights_.size() << 1) + 1);
        }

        weights_.at(index) = Node{
            .symbol = node_visited->symbol,
            .frequency = node_visited->frequency,
        };

        if (node_visited->left) {
          to_visit.push({Left(index), node_visited->left});
        }

        if (node_visited->right) {
          to_visit.push({Right(index), node_visited->right});
        }
      };
    };

    tree_to_vector(min_heap.top());
    AssignCodesFromTree(0, {});
  }

  std::vector<bool> GetCode(char chr) { return code_map_[chr]; }

  std::string Translate(const std::vector<std::byte>& code) {
    if (!code.size()) {
      return "";
    }

    // Tree node
    size_t cur_node = 0;
    auto select_branch = [this, &cur_node](const auto& chunk, uint8_t bit_idx) {
      if (std::to_integer<uint8_t>(chunk) & (1U << bit_idx)) {
        cur_node = Right(cur_node);
      } else {
        cur_node = Left(cur_node);
      }
    };

    std::string original_text{};
    auto analyze_byte = [this, &cur_node, &original_text, &select_branch](
                            const auto& chunk, uint8_t bit_end) {
      for (int bit_idx = 7; bit_idx >= bit_end; --bit_idx) {
        if (cur_node >= weights_.capacity()) {
          //TODO(Marcos): Implement error handling
          break;
        }

        if (IsLeaf(weights_.at(cur_node))) {
          original_text.push_back(weights_.at(cur_node).symbol.value());
          cur_node = 0;
        }

        select_branch(chunk, bit_idx);
      }
    };

    size_t chunck_idx = 1;
    while (chunck_idx < code.size() - 1) {
      const auto& chunck = code.at(chunck_idx);
      analyze_byte(chunck, 0);

      ++chunck_idx;
    }

    const auto& chunck = code.at(chunck_idx);

    // TODO(Marcos): Check if header is valid. It should only contains values from 0 to 7
    uint8_t last_bits = static_cast<uint8_t>(code.at(0));
    analyze_byte(chunck, (7 - last_bits));

    return original_text;
  }

  friend std::ostream& operator<<(std::ostream&, const CodingTree&);
  friend std::istream& operator>>(std::istream&, CodingTree&);

 private:
  inline int32_t Right(int32_t parent) const { return (parent << 1) + 2; }
  inline int32_t Left(int32_t parent) const { return (parent << 1) + 1; }
  inline bool IsLeaf(const Node& node) const { return node.symbol.has_value(); }

  void AssignCodesFromTree(size_t node, std::vector<bool>&& path) {
    if (node >= weights_.size()) {
      return;
    }

    if (IsLeaf(weights_.at(node))) {
      code_map_[weights_.at(node).symbol.value()] = path;
      return;
    }

    path.push_back(false);
    AssignCodesFromTree(Left(node), std::move(path));
    path.pop_back();
    path.push_back(true);
    AssignCodesFromTree(Right(node), std::move(path));
    path.pop_back();
  }

  std::vector<Node> weights_{};
  std::map<char, std::vector<bool>> code_map_{};
};

}  // namespace

struct StaticHuffmanCode::StaticHuffmanCodeImpl {
  CodingTree coding_tree{};

  std::vector<std::byte> Encode(const std::span<char>& text) {
    auto frequencies = GetFrequencies(text);
    coding_tree.BuildTree(frequencies);

    std::vector<std::byte> encoded{};
    // Add header space
    encoded.emplace_back(std::byte{});

    std::byte buffer{0b00000000};
    uint8_t cur_bit = 0;

    auto fill_buffer = [&encoded, &cur_bit,
                        &buffer](const std::vector<bool>& bit_vector) {
      for (const auto& bit : bit_vector) {
        buffer |= static_cast<std::byte>(bit)
                  << (7 - cur_bit);  // turns on/off current bit

        ++cur_bit;
        if (cur_bit == 8) {
          encoded.emplace_back(buffer);
          buffer ^= buffer;  // sets buffer to zero
          cur_bit = 0;
        }
      }
    };

    for (const auto& chr : text) {
      auto bit_vector = coding_tree.GetCode(chr);
      fill_buffer(bit_vector);
    }

    encoded.emplace_back(buffer);

    // Saves the number of bits on the last byte and placed it on the header
    encoded.at(0) = std::byte{cur_bit};

    return encoded;
  }

  std::string Decode(const std::vector<std::byte>& encoded) {
    return coding_tree.Translate(encoded);
  }

  std::map<char, int32_t> GetFrequencies(const std::span<char>& source) {
    std::map<char, int32_t> frequency_map{};

    for (const auto& character : source) {
      if (!frequency_map.contains(character)) {
        frequency_map[character] = 1;
      } else {
        ++frequency_map[character];
      }
    }

    return frequency_map;
  }
};

StaticHuffmanCode::StaticHuffmanCode()
    : impl_(std::make_unique<StaticHuffmanCodeImpl>()) {}

StaticHuffmanCode::~StaticHuffmanCode() = default;

std::vector<std::byte> StaticHuffmanCode::Encode(const std::span<char>& text) {
  return impl_->Encode(text);
}

std::string StaticHuffmanCode::Decode(const std::vector<std::byte>& encoded) {
  return impl_->Decode(encoded);
}

}  // namespace doht::shc