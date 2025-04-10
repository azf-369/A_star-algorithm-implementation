// 启发式函数，使用hamming距离：
double heuristic(const Point& a, const Point& b) {
    return abs(a.x - b.x) + abs(a.y - b.y);
}

// 使用膨胀算法：
bool expansion_pos_check(const Point& pos) {
    vector<Point> expansion_pos = {
        {3, 3}, {3, 2}, {3, 1}, {3, 0}, {3, -1}, {3, -2}, {3, -3},
        {2, 3}, {2, 2}, {2, 1}, {2, 0}, {2, -1}, {2, -2}, {2, -3},
        {1, -3}, {1, -2}, {1, -1}, {1, 0}, {1, 1}, {1, 2}, {1, 3},
        {0, -3}, {0, -2}, {0, -1}, {0, 0}, {0, 1}, {0, 2}, {0, 3},
        {-1, -3}, {-1, -2}, {-1, -1}, {-1, 0}, {-1, 1}, {-1, 2}, {-1, 3},
        {-2, -3}, {-2, -2}, {-2, -1}, {-2, 0}, {-2, 1}, {-2, 2}, {-2, 3},
        {-3, -3}, {-3, -2}, {-3, -1}, {-3, 0}, {-3, 1}, {-3, 2}, {-3, 3}
    };
    for (const auto& p : expansion_pos) {
        Point new_pos = pos + p;
        if(Maptest.at<uchar>(new_pos.x, new_pos.y) == 0){
            return false;
        }
    }
    return true;
}

// A* 算法实现
int Astar(const Point& start, const Point& target, vector<Point>& PathList) {
    int cost=0;

    // 标记已经加入过的节点
    unordered_map<Point, Path_Node*, pair_hash> open_list_map;
    
    // 定义移动方向（上下左右和对角线）
    vector<Point> directions = {
        {0, 1}, {1, 0}, {0, -1}, {-1, 0}, {-1, -1}, {1, 1}, {-1, 1}, {1, -1}
    };

    // 初始化起点和终点
    Path_Node* start_node = new Path_Node(start, 0.0, heuristic(start, target), nullptr);
    Path_Node* target_node = new Path_Node(target, 0.0, 0.0, nullptr);

    // 使用优先队列存储开放列表
    priority_queue<Path_Node*, vector<Path_Node*>, ComparePathNode> open_list;
    open_list.push(start_node);
    open_list_map[start_node->position] = start_node;

    // 使用 unordered_map 存储已访问的节点
    unordered_map<Point, Path_Node*, pair_hash> closed_list;

    while (!open_list.empty()) {
        Path_Node* current = open_list.top();
        open_list.pop();

        // 如果到达终点，构建路径并返回
        if (current->position == target) {
            vector<Point> path;
            while (current) {
                PathList.push_back(current->position);
                current = current->parent;
                cost++;
            }
            reverse(PathList.begin(), PathList.end());

            // 释放 open_list_map 中的所有节点
            for (auto& pair : open_list_map) {
                delete pair.second;
            }
            return cost;
        }

        // 将当前节点加入关闭列表
        closed_list[current->position] = current;

        // 遍历所有可能的移动方向
        for (const auto& dir : directions) {
            Point neighbor_pos = current->position + dir;

            // 检查邻居节点是否在地图范围内且不是障碍物
            if (neighbor_pos.x >= 0 && neighbor_pos.x < MapParam.width &&
                neighbor_pos.y >= 0 && neighbor_pos.y < MapParam.height &&
                expansion_pos_check(neighbor_pos)) {
                // 检查是否已经在关闭列表中
                if (closed_list.find(neighbor_pos) != closed_list.end()) {
                    continue;
                }
                
                // 更新节点代价的逻辑
                auto it = open_list_map.find(neighbor_pos);
                if (it != open_list_map.end()) {
                    Path_Node* existing_node = it->second;
                    double g_new = current->g + sqrt(pow(dir.x,2)+pow(dir.y,2)); //sqrt(pow(dir.x,2)+pow(dir.y,2)) / 1.0
                    if (g_new < existing_node->g) {
                        open_list_map.erase(it);

                        // 插入新节点
                        existing_node->g = g_new;
                        existing_node->f = g_new + existing_node->h;
                        existing_node->parent = current;
                        open_list.push(existing_node);
                        open_list_map[neighbor_pos] = existing_node;
                    }
                    continue;
                }

                // 计算邻居节点的代价
                double g_new = current->g + sqrt(pow(dir.x,2)+pow(dir.y,2));  // sqrt(pow(dir.x,2)+pow(dir.y,2)) / 1.0
                double h_new = heuristic(neighbor_pos, target);
                Path_Node* neighbor_node = new Path_Node(neighbor_pos, g_new, h_new, current);

                // 将邻居节点加入开放列表
                open_list.push(neighbor_node);
                open_list_map[neighbor_node->position] = neighbor_node;
            }
        }
    }

    cout << "No path found!" << endl;
    // 释放 open_list_map 中的所有节点
    for (auto& pair : open_list_map) {
        delete pair.second;
    }
    return -1;
}
