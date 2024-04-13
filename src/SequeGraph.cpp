#ifndef  _SequeGraph_cpp_
#define _SequeGraph_cpp_

#include<stdlib.h>
#include<algorithm>
#include"SequeGraph.h"
#include<time.h>

// 构造函数:建立一个空的邻接表
SequeGraph::SequeGraph(int sz):maxVertices(sz), numVertices(0), numEdges(0), nodeTable(vector<Vertex>(maxVertices))
{
    if (nodeTable.empty()) {
        cerr << "存储空间分配错误！" << endl;
        exit(1);
    }
    for (int i = 0; i < maxVertices; ++i)
        nodeTable[i].adj = NULL;
}

SequeGraph::SequeGraph(const SequeGraph& graph)
{
    maxVertices = graph.maxVertices;
    numVertices = graph.numVertices;
    numEdges = 0;
    //nodeTable = new Vertex[maxVertices]; // 创建顶点表数组
    for (int i = 0; i < graph.numVertices; i++)
    {
        Vertex temp;
        temp.work = graph.nodeTable[i].work;
        temp.time = graph.nodeTable[i].time;
        //temp.inDegree = graph.nodeTable[i].inDegree;
        //temp.outDegree = graph.nodeTable[i].outDegree;
        temp.NextVertexs = graph.nodeTable[i].NextVertexs;
        temp.FrontVertexs = graph.nodeTable[i].FrontVertexs;
        temp.adj = NULL;
        nodeTable.push_back(temp);
    }
    for (int i = 0; i < graph.numVertices; i++)
    {
        if (graph.nodeTable[i].outDegree)
        {
            Edge* q = graph.nodeTable[i].adj;
            while (q)
            {
                this->insertEdge(getValue(i), getValue(q->dest));
                q = q->link;
            }
        }
    }
}

// 析构函数
SequeGraph::~SequeGraph() {
    // 删除各边链表中的结点
    for (int i = 0; i < numVertices; i++) {
        Edge* p = nodeTable[i].adj; // 找到其对应链表的首结点
        while (p != NULL) { // 不断地删除第一个结点
            nodeTable[i].adj = p->link;
            delete p;
            p = nodeTable[i].adj;
        }
    }
    //delete nodeTable; // 删除顶点表数组
}

SequeGraph& SequeGraph::operator=(SequeGraph& Graph)
{
    this->maxVertices = Graph.maxVertices;
    this->numVertices = Graph.numVertices;
    //nodeTable = new Vertex[maxVertices]; // 创建顶点表数组
    for (int i = 0; i < Graph.numVertices; i++)
    {
        //int work = Graph.nodeTable[i].work;
        //int time = Graph.nodeTable[i].time;
        //char direction = Graph.nodeTable[i].direction;
        //this->insertVertex(work, time, direction);
        Vertex temp;
        temp.work = Graph.nodeTable[i].work;
        temp.time = Graph.nodeTable[i].time;
        temp.direction = Graph.nodeTable[i].direction;
        temp.setupTime = Graph.nodeTable[i].setupTime;
        temp.NextVertexs = Graph.nodeTable[i].NextVertexs;
        temp.FrontVertexs = Graph.nodeTable[i].FrontVertexs;
        temp.adj = NULL;
        if (i < nodeTable.size())
        {
            nodeTable[i] = temp;
        }
        else
        {
            nodeTable.push_back(temp);
        }
    }
    if (numEdges == 0)
    {
        for (int i = 0; i < Graph.numVertices; i++)
        {
            if (Graph.nodeTable[i].outDegree)
            {
                Edge* q = Graph.nodeTable[i].adj;
                while (q)
                {
                    insertEdge(getValue(i), getValue(q->dest));
                    q = q->link;
                }
            }
        }
    }
    return *this;
}

// 建立邻接表表示的图
void SequeGraph::inputGraph() {

    int n, m;                                // 存储顶点树和边数
    int work, time;                     // 作业节点
    char direction;
    int e1, e2;                             // 边
    int i, j, k;

    cout << "请输入顶点数和边数：" << endl;
    cin >> n >> m;

    cout << "请输入各作业序号和时间：" << endl;
    for (i = 0; i < n; i++) {
        cin >> work >> time >> direction;
        insertVertex(work, time, direction);           // 插入顶点
    }

    cout << "请输入图的各边的信息：" << endl;
    i = 0;
    while (i < m) {
        cin >> e1 >> e2;
        j = getVertexPos(e1);
        k = getVertexPos(e2);
        if (j == -1 || k == -1)
            cout << "边两端点信息有误，请重新输入！" << endl;
        else {
            insertEdge(e1, e2); // 插入边
            i++;
        }
    } // while
}

// 输出有向图中的所有顶点和边信息
void SequeGraph::outputGraph() {
    int n, m, i;
    int e1, e2;                                 // 顶点
    Edge* p;

    n = numVertices;
    m = numEdges;
    cout << "图中的顶点数为" << n << ",边数为" << m << endl;
    for (i = 0; i < n; i++) {
        p = nodeTable[i].adj;
        while (p != NULL) {
            e1 = getValue(i); // 有向边<i, p->dest>
            e2 = getValue(p->dest);
            cout << "<" << e1 << ", " << e2  << ">" << endl;
            p = p->link; // 指向下一个邻接顶点
        }
    }
}

void SequeGraph::printself()
{
    cout << "The number of vertexs and edges is: " << "numVertexs = " << numVertices << "  numEdges = " << numEdges << endl;
    for (int i = 0; i < numVertices; i++)
    {
        cout<< "( " << this->nodeTable[i].work << " , " << this->nodeTable[i].time <<" , "<<nodeTable[i].inDegree << " , " << nodeTable[i].outDegree  << " ) ";
        Edge* p = this->nodeTable[i].adj;
        while (p != NULL)
        {
            cout << nodeTable[p->dest].work<<"  ";
            p = p->link;
        }
        cout << endl;
    }
}

// 取位置为i的顶点中的作业序号
int SequeGraph::getValue(int i) {
    if (i >= 0 && i < numVertices)
        return nodeTable[i].work;
    return NULL;
}

// 插入顶点
bool SequeGraph::insertVertex(const int work, const int time , const char direction) {
    if (numVertices == maxVertices)                   // 顶点表满，不能插入
    {
        cout << "the vertex table fulled!" << endl;
        return false;
    }
    else
    {
        nodeTable[numVertices].work = work;       // 插入在表的最后
        nodeTable[numVertices].time = time;
        nodeTable[numVertices].direction = direction;
        nodeTable[numVertices].inDegree = 0;
        nodeTable[numVertices].outDegree = 0;
        nodeTable[numVertices].setupTime = 0;
        numVertices++;
    }

    return true;
}

// 插入边
bool SequeGraph::insertEdge(int work1, int work2) {
    int v1 = getVertexPos(work1);
    int v2 = getVertexPos(work2);
    nodeTable[v1].outDegree++;
    nodeTable[v2].inDegree++;
    if (v1 >= 0 && v1 < numVertices && v2 >= 0 && v2 < numVertices) {
        Edge* p = nodeTable[v1].adj;                    // v1对应的边链表头指针
        while (p != NULL && p->dest != v2)       // 寻找邻接顶点v2
            p = p->link;
        if (p != NULL)                                           // 已存在该边，不插入
        {
            cout << "this edge already existed!";
            return false;
        }
        p = new Edge();                                             // 创建新边结点
        p->dest = v2;
        p->link = nodeTable[v1].adj;                      // 链入v1边链表首位
        nodeTable[v1].adj = p;
        numEdges++;
        return true;
    }
    return false;
}

// 有向图删除顶点较麻烦
bool SequeGraph::removeVertex(int work) {
    int v = getVertexPos(work);
    if (numVertices < 1 || v < 0 || v > numVertices)
        return false; // 表空或顶点号超出范围

    Edge* p, * s;

    // 1.清除顶点v的边链表结点w 边<v,w>（即delete 以 v 为起点的边）
    while (nodeTable[v].adj != NULL) 
    {
        p = nodeTable[v].adj;
        nodeTable[v].adj = p->link;
        if(p->dest>=0 && p->dest < numVertices)
            nodeTable[p->dest].inDegree--;
        nodeTable[v].outDegree--;
        delete p;
        numEdges--; // 与顶点v相关联的边数减1
    } // while结束

    // 2.清除<w, v>，与v有关的边（即 delete 以 v 为终点的边）
    for (int i = 0; i < numVertices; i++) {
        if (i != v)
        { // 不是当前顶点v
            s = NULL;
            p = nodeTable[i].adj;
            while (p != NULL && p->dest != v) 
            {                                       // 在顶点i的链表中找v的顶点
                s = p;
                p = p->link;                 // 往后找
            }
            if (p != NULL) {             // 找到了v的结点
                if (s == NULL)           // 说明p是nodeTable[i].adj
                {         
                    nodeTable[i].adj = p->link;
                }
                else {
                    s->link = p->link; // 保存p的下一个顶点信息
                }
                nodeTable[i].outDegree--;
                nodeTable[v].inDegree--;
                delete p; // 删除结点p
                numEdges--; // 与顶点v相关联的边数减1
            }
        }
    }
    numVertices--; // 图的顶点个数减1
    
    //3.将尾端numVertics填补到被删除的位置
    nodeTable[v].work = nodeTable[numVertices].work; // 此时numVertices，比原来numVertices小1，所以，这里不需要numVertices-1
    nodeTable[v].time = nodeTable[numVertices].time;
    nodeTable[v].direction = nodeTable[numVertices].direction;
    nodeTable[v].inDegree = nodeTable[numVertices].inDegree;
    nodeTable[v].outDegree = nodeTable[numVertices].outDegree;
    nodeTable[v].FrontVertexs = nodeTable[numVertices].FrontVertexs;
    nodeTable[v].NextVertexs = nodeTable[numVertices].NextVertexs;
    nodeTable[v].adj = nodeTable[numVertices].adj;

    // 4.要将填补的顶点对应的位置改写
    for (int i = 0; i < numVertices; i++) {
        p = nodeTable[i].adj;
        while (p != NULL && p->dest != numVertices) // 在顶点i的链表中找numVertices的顶点
            p = p->link;                                                       // 往后找
        if (p != NULL)                                                      // 找到了numVertices的结点
            p->dest = v;                                                       // 将邻接顶点numVertices改成v
    }

    //5.重置容器中位于numVertics的顶点数据
    nodeTable[numVertices].work = 0;
    nodeTable[numVertices].time = 0;
    nodeTable[numVertices].inDegree = 0;
    nodeTable[numVertices].outDegree = 0;
    nodeTable[numVertices].direction = NULL;

    return true;
}

// 删除边
bool SequeGraph::removeEdge(int work1, int work2) {
    int v1 = getVertexPos(work1);
    int v2 = getVertexPos(work2);
    if (v1 != -1 && v2 != -1) {
        Edge* p = nodeTable[v1].adj, * q = NULL;
        while (p != NULL && p->dest != v2) { // v1对应边链表中找被删除边
            q = p;
            p = p->link;
        }
        if (p != NULL) { // 找到被删除边结点
            if (q == NULL) // 删除的结点是边链表的首结点
                nodeTable[v1].adj = p->link;
            else
                q->link = p->link; // 不是，重新链接
            delete p;
            return true;
        }
    }
    return false; // 没有找到结点
}

bool SequeGraph::setVertexSetuptime(const int work, const int setupTime)
{
    if (getVertexPos(work) + 1)
    {
        this->nodeTable[getVertexPos(work)].setupTime = setupTime;
        return true;
    }
    return false;
}

bool SequeGraph::setOutDegree(const int work, const int OutDegree)
{
    if (getVertexPos(work) != -1)
    {
        this->nodeTable[getVertexPos(work)].outDegree = OutDegree;
        return true;
    }
    return false;
}

bool SequeGraph::setInDegree(const int work, const int InDegree)
{
    if (getVertexPos(work) != -1)
    {
        this->nodeTable[getVertexPos(work)].inDegree = InDegree;
        return true;
    }
    return false;
}

bool SequeGraph::resetVertexDirection(const int work, const char direction)
{
    if (getVertexPos(work) + 1)
    {
        this->nodeTable[getVertexPos(work)].direction = direction;
        return true;
    }
    return false;
}

// 取顶点v的第一个邻接顶点
int SequeGraph::getFirstNeighbor(int work) {
    int v = getVertexPos(work);
    if (v != -1) {
        Edge* p = nodeTable[v].adj; // 对应链表第一个边结点
        if (p != NULL) // 存在，返回第一个邻接顶点
            return p->dest;
    }
    return -1; // 第一个邻接顶点不存在
}

// 取顶点v的邻接顶点w的下一邻接顶点
int SequeGraph::getNextNeighbor(int v, int w)
{
    if (v != -1) {
        Edge* p = nodeTable[v].adj;                 // 对应链表第一个边结点
        while (p != NULL && p->dest != w)   // 寻找邻接顶点w
            p = p->link;
        if (p != NULL && p->link != NULL)
            return p->link->dest;              // 返回下一个邻接顶点
    }
    return -1;                                         // 下一个邻接顶点不存在
}

// 给出顶点vertex在图中的位置
int SequeGraph::getVertexPos(const int vertex) {
    for (int i = 0; i < numVertices; i++)
        if (nodeTable[i].work == vertex)
            return i;
    return -1;
}

int SequeGraph::getOutDegree(const int work)
{
    return nodeTable[getVertexPos(work)].outDegree;
}

int SequeGraph::getInDegree(const int work)
{
    return nodeTable[getVertexPos(work)].inDegree;
}

int SequeGraph::getTime(const int work)
{
    return nodeTable[getVertexPos(work)].time;
}

int SequeGraph::getSetupTime(const int work)
{
    return nodeTable[getVertexPos(work)].setupTime;
}

char SequeGraph::getDirection(const int work)
{
    return nodeTable[getVertexPos(work)].direction;
}

// 当前顶点数
int SequeGraph::numberOfVertices() {
    return numVertices;
}

set<int>& SequeGraph::getFrontVertexs(const int work)
{
    int v = getVertexPos(work);
    Edge *p;
    for (int i = 0; i < numVertices; i++)
    {
        if (i != v)                              // 不是当前顶点v
        {
            p = nodeTable[i].adj;
            while (p != NULL && p->dest != v)
            {                                       // 在顶点i的链表中找v的顶点
                p = p->link;                 // 往后找
            }
            if (p != NULL)               // 找到了v的结点( p->dest == v )
            {            
                int work = nodeTable[i].work;
                nodeTable[v].FrontVertexs.insert(work);
            }
        }
    }
    return nodeTable[v].FrontVertexs;
}

set<int>& SequeGraph::getNextVertexs(const int work)
{
    int v = getVertexPos(work);
    Edge* p = nodeTable[v].adj;
    while (p != NULL)
    {
        nodeTable[v].NextVertexs.insert(getValue(p->dest));
        p = p->link;
    }
    return nodeTable[v].NextVertexs;
}

set<int> SequeGraph::getNoFrontVertexs()
{
    set<int> NoFront;
    for (int i = 0; i < numVertices; i++)
    {
        if (nodeTable[i].inDegree == 0)
        {
            NoFront.insert(getValue(i));
        }
    }
    return NoFront;
}

vector<pair<int, int>> SequeGraph::getPostWork(int work)
{
    vector<pair<int, int>> postWorkSet;
    for (auto& x : getNextVertexs(work))
    {
        postWorkSet.push_back(pair<int, int>(x, getTime(x)));
    }
    return postWorkSet;
}

vector<int> SequeGraph::topoSort()
{
    vector<int> topo;
    while (numVertices)
    {
        set<int> tempset = this->getNoFrontVertexs();
        if (!tempset.empty())
        {
            set<int>::const_iterator it(tempset.begin());
            advance(it, rand() % (tempset.size()));
            topo.push_back(*it);
            this->removeVertex(*it);
        }
        //this->printself();
    }
    return topo;
}


#endif // ! _SequeGraph_cpp_