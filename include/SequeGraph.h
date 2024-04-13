#ifndef _SequeGraph_h_
#define _SequeGraph_h_

#include <iostream>
#include<set>
#include<vector>
using namespace std;

    const int DefaultVertices = 300;
    //const int maxValue = 100000; // 代表无穷大的值（=∞）

    struct Edge {               // 边结点的定义
        int dest;                   // 边的另一顶点位置
        Edge* link;             // 下一条边链指针
    };

    struct Vertex {             // 顶点的定义
        int work;                  // 作业的编号
        int time;                   // 作业的时间
        int setupTime{};          // 作业开始时间
        char direction;
        int outDegree{};
        int inDegree{};
        set<int> FrontVertexs;
        set<int> NextVertexs;
        Edge* adj;               // 边链表的头指针
    };

    class SequeGraph {
    public:
        SequeGraph(int sz = DefaultVertices); // 构造函数
        SequeGraph(const SequeGraph&);
        ~SequeGraph();                                    // 析构函数

        SequeGraph& operator = (SequeGraph&);

        void inputGraph(); // 建立邻接表表示的图
        void outputGraph(); // 输出图中的所有顶点和边信息
        void printself();

        bool insertVertex(const int work, const int time, const char direction); // 插入顶点
        bool insertEdge(int work1, int work2); // 插入边
        bool removeVertex(int v); // 删除顶点
        bool removeEdge(int v1, int v2); // 删除边

        bool setVertexSetuptime(const int work, const int setupTime);
        bool setOutDegree(const int work, const int OutDegree);
        bool setInDegree(const int work, const int InDegree);
        bool resetVertexDirection(const int work, const char direction);

        int getValue(int i);                           // 取位置为i的顶点中的值
        int getFirstNeighbor(int v);             // 取顶点v的第一个邻接顶点
        int getNextNeighbor(int v, int w);  // 取顶点v的邻接顶点w的下一邻接顶点
        int getVertexPos(const int work);   // 给出顶点vertex在图中的位置
        int getOutDegree(const int work);
        int getInDegree(const int work);
        int getTime(const int work);
        int getSetupTime(const int work);
        char getDirection(const int work);

        int numberOfVertices(); // 当前顶点数
        inline int getNumOfVertices() {
            return numVertices;
        }

        set<int>& getFrontVertexs(const int work);
        set<int>& getNextVertexs(const int work);
        set<int> getNoFrontVertexs();

        inline vector<Vertex>& getVertexSet() {
            return nodeTable;
        }
        vector<pair<int, int>> getPostWork(int work);

        vector<int> topoSort();
    private:
        int maxVertices; // 图中最大的顶点数
        int numEdges; // 当前边数
        int numVertices; // 当前顶点数
        vector<Vertex> nodeTable; // 顶点表(各边链表的头结点)
    };

#endif _SequeGraph_h_