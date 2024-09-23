// Doth Library (C) by Marcos Oliveira

#include "doht/ahc.h"

#include <assert.h>

#include <bitset>
#include <list>
#include <map>
#include <memory>
#include <optional>
#include <set>
#include <span>
#include <string>
#include <vector>

namespace doht {
namespace {

struct IVitterAdaptativeNode {
  using Node = IVitterAdaptativeNode;
  Node* left{nullptr};
  Node* right{nullptr};
  Node* parent{nullptr};
  int32_t weight{};
  std::optional<char> symbol{};

  IVitterAdaptativeNode(int32_t w, const std::optional<char>& s,
                        Node* left = nullptr, Node* right = nullptr,
                        Node* parent = nullptr) {
    this->weight = w;
    this->symbol = s;
    this->parent = parent;
    this->left = left;
    this->right = right;
  }
};

struct VitterAdaptativeNode : public IVitterAdaptativeNode {
  VitterAdaptativeNode(int32_t w, const std::optional<char>& s,
                       Node* left = nullptr, Node* right = nullptr,
                       Node* parent = nullptr)
      : IVitterAdaptativeNode(w, s, left, right, parent) {}
};

struct VitterAdaptativeNytNode : public IVitterAdaptativeNode {
  std::set<char> not_transmitted{};

  VitterAdaptativeNytNode(const std::set<char>& allowed_character)
      : IVitterAdaptativeNode(0, std::nullopt) {
    this->not_transmitted = allowed_character;
  }
};

class VitterAdaptativeTree {
 public:
  explicit VitterAdaptativeTree(const std::set<char>& allowed_characters) {
    nodes_.emplace_back(std::move(
        std::make_unique<VitterAdaptativeNytNode>(allowed_characters)));
    root_ = nodes_.front().get();
    nyt_ = root_;
    for (const auto& characters : allowed_characters) {
      leafs_[characters] = nyt_;
    }
  }

  std::vector<bool> Encode(char symbol) {
    IVitterAdaptativeNode* node = leafs_[symbol];

    if (node == nyt_) {
      std::unique_ptr<IVitterAdaptativeNode> new_leaf =
          std::make_unique<VitterAdaptativeNode>(1,
                                                 std::optional<char>(symbol));
      std::unique_ptr<IVitterAdaptativeNode> parent =
          std::make_unique<VitterAdaptativeNode>(1, std::nullopt);
      parent->left = nyt_;
      nyt_->parent = parent.get();
      parent->right = new_leaf.get();
      new_leaf->parent = parent.get();

      nodes_.emplace_back(std::move(new_leaf));
      nodes_.emplace_back(std::move(parent));
    } else {
      node->weight += 1;
    }
  }

  std::string Decode(const std::vector<bool>& encoded) { return ""; }

 private:
  std::map<char, IVitterAdaptativeNode*> leafs_;
  std::map<char, std::vector<bool>> codewords_;
  IVitterAdaptativeNode* nyt_{nullptr};
  IVitterAdaptativeNode* root_{nullptr};
  std::list<std::unique_ptr<IVitterAdaptativeNode>> nodes_{};
};

}  // namespace

struct AdaptativeHuffmanCode::AdaptativeHuffmanCodeImpl {

  AdaptativeHuffmanCodeImpl(const std::set<char>& allowed_characters)
      : ahc(allowed_characters) {}

  std::vector<bool> Encode(const std::span<char>& text) {
    std::vector<bool> encoded{};

    for (const auto& character : text) {
      auto codeword = ahc.Encode(character);
      for (const auto& bit : codeword) {
        encoded.push_back(bit);
      }
    }

    return encoded;
  }

  std::string Decode(const std::vector<bool>& encoded) {
    return ahc.Decode(encoded);
  }

  VitterAdaptativeTree ahc;
};

AdaptativeHuffmanCode::AdaptativeHuffmanCode(
    const std::set<char>& allowed_characters)
    : impl_(std::make_unique<AdaptativeHuffmanCodeImpl>(allowed_characters)) {}

AdaptativeHuffmanCode::~AdaptativeHuffmanCode() {}

std::vector<bool> AdaptativeHuffmanCode::Encode(const std::span<char>& text) {
  return impl_->Encode(text);
}

std::string AdaptativeHuffmanCode::Decode(const std::vector<bool>& encoded) {
  return impl_->Decode(encoded);
}

}  // namespace doht