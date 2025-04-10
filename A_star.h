#ifndef A_STAR_H
#define A_STAR_H

// 定义一个结构体用于存储节点信息
struct Path_Node {
    Point position;  // 当前节点的位置
    double g;   // 从起点到当前节点的实际代价
    double h;   // 启发式函数的值（到终点的估计代价）
    double f;   // f = g + h
    Path_Node* parent;  // 父节点

    Path_Node(Point p, double g_value, double h_value, Path_Node* parent_node)
        : position(p), g(g_value), h(h_value), f(g_value + h_value), parent(parent_node) {}

    // 用于优先队列的比较
    bool operator>(const Path_Node& other) const {
        if (f == other.f) {
            return g > other.g;
        }
        return f > other.f;
    }
};

struct ComparePathNode {
    bool operator()(const Path_Node* a, const Path_Node* b) const {
        return a->f > b->f || (a->f == b->f && a->g > b->g); // 按照 f 值从小到大排序
    }
};

// 自定义哈希函数用于 Point 类型
struct pair_hash {
    inline size_t operator()(const Point& v) const {
        return v.x * 31 + v.y;
    }
};

#endif
