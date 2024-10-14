// tree_path.cpp - インデックスリストを用いて木構造を歩き回る
#include <vector>
#include <memory>
#include <cassert>
#include <cstdio>

// ツリー構造
struct tree_t;
typedef std::shared_ptr<tree_t> treeptr_t;
struct tree_t : std::vector<treeptr_t> { };

// インデックスリスト
typedef std::vector<size_t> index_list_t;

// インデックスリストを出力
void print_index_list(const index_list_t& indeces)
{
    printf("{ ");
    for (size_t i = 0; i < indeces.size(); ++i)
    {
        if (i == 0)
            printf("%d", (int)indeces[i]);
        else
            printf(", %d", (int)indeces[i]);
    }
    printf(" }\n");
}

// インデックスリストを解決
treeptr_t resolve_index_list(treeptr_t root, const index_list_t& indices) {
    treeptr_t node = root;
    for (size_t idx : indices) {
        if (!node || idx >= node->size()) {
            return nullptr;
        }
        node = (*node)[idx];
    }
    return node;
}

// 次のインデックスリストを計算
index_list_t next_index_list(treeptr_t root, index_list_t indices) {
    if (!root) {
        return {}; // ルートが無効の場合
    }

    if (indices.empty() && root->size() > 0) {
        return {0}; // 最初の要素を指す
    }

    treeptr_t node = resolve_index_list(root, indices);
    if (!node) {
        return {}; // 無効なインデックスリストの場合
    }

    if (node->size() > 0) {
        indices.push_back(0);
        return indices; // 現在のノードに子ノードが存在する場合
    }

    // 子ノードが存在しない場合、親に戻って次の兄弟ノードを指す
    while (!indices.empty()) {
        size_t last_index = indices.back();
        indices.pop_back();

        treeptr_t parent_node = resolve_index_list(root, indices);
        if (last_index + 1 < parent_node->size()) {
            indices.push_back(last_index + 1);
            return indices;
        }
    }

    return indices; // 最後の要素に到達した場合、空を返す
}

// メイン関数
int main(int argc, char **argv) {
    // ツリー構造を構築
    auto root = std::make_shared<tree_t>();
    for (int i = 0; i < 1; ++i) {
        auto child = std::make_shared<tree_t>();
        for (int j = 0; j < 3; ++j) {
            auto grandchild = std::make_shared<tree_t>();
            for (int j = 0; j < 2; ++j) {
                grandchild->push_back(std::make_shared<tree_t>());
            }
            child->push_back(grandchild);
        }
        root->push_back(child);
    }

    // 適当にテスト
    index_list_t path = {};
    auto result = next_index_list(root, path);
    print_index_list(result);
    for (int i = 0; i < 20; ++i) {
        result = next_index_list(root, result);
        print_index_list(result);
    }

    return 0;
}
