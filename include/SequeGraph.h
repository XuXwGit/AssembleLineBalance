#ifndef _SequeGraph_h_
#define _SequeGraph_h_

#include <iostream>
#include<set>
#include<vector>
using namespace std;

    const int DefaultVertices = 300;
    //const int maxValue = 100000; // ����������ֵ��=�ޣ�

    struct Edge {               // �߽��Ķ���
        int dest;                   // �ߵ���һ����λ��
        Edge* link;             // ��һ������ָ��
    };

    struct Vertex {             // ����Ķ���
        int work;                  // ��ҵ�ı��
        int time;                   // ��ҵ��ʱ��
        int setupTime{};          // ��ҵ��ʼʱ��
        char direction;
        int outDegree{};
        int inDegree{};
        set<int> FrontVertexs;
        set<int> NextVertexs;
        Edge* adj;               // �������ͷָ��
    };

    class SequeGraph {
    public:
        SequeGraph(int sz = DefaultVertices); // ���캯��
        SequeGraph(const SequeGraph&);
        ~SequeGraph();                                    // ��������

        SequeGraph& operator = (SequeGraph&);

        void inputGraph(); // �����ڽӱ��ʾ��ͼ
        void outputGraph(); // ���ͼ�е����ж���ͱ���Ϣ
        void printself();

        bool insertVertex(const int work, const int time, const char direction); // ���붥��
        bool insertEdge(int work1, int work2); // �����
        bool removeVertex(int v); // ɾ������
        bool removeEdge(int v1, int v2); // ɾ����

        bool setVertexSetuptime(const int work, const int setupTime);
        bool setOutDegree(const int work, const int OutDegree);
        bool setInDegree(const int work, const int InDegree);
        bool resetVertexDirection(const int work, const char direction);

        int getValue(int i);                           // ȡλ��Ϊi�Ķ����е�ֵ
        int getFirstNeighbor(int v);             // ȡ����v�ĵ�һ���ڽӶ���
        int getNextNeighbor(int v, int w);  // ȡ����v���ڽӶ���w����һ�ڽӶ���
        int getVertexPos(const int work);   // ��������vertex��ͼ�е�λ��
        int getOutDegree(const int work);
        int getInDegree(const int work);
        int getTime(const int work);
        int getSetupTime(const int work);
        char getDirection(const int work);

        int numberOfVertices(); // ��ǰ������
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
        int maxVertices; // ͼ�����Ķ�����
        int numEdges; // ��ǰ����
        int numVertices; // ��ǰ������
        vector<Vertex> nodeTable; // �����(���������ͷ���)
    };

#endif _SequeGraph_h_