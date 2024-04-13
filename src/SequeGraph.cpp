#ifndef  _SequeGraph_cpp_
#define _SequeGraph_cpp_

#include<stdlib.h>
#include<algorithm>
#include"SequeGraph.h"
#include<time.h>

// ���캯��:����һ���յ��ڽӱ�
SequeGraph::SequeGraph(int sz):maxVertices(sz), numVertices(0), numEdges(0), nodeTable(vector<Vertex>(maxVertices))
{
    if (nodeTable.empty()) {
        cerr << "�洢�ռ�������" << endl;
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
    //nodeTable = new Vertex[maxVertices]; // �������������
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

// ��������
SequeGraph::~SequeGraph() {
    // ɾ�����������еĽ��
    for (int i = 0; i < numVertices; i++) {
        Edge* p = nodeTable[i].adj; // �ҵ����Ӧ������׽��
        while (p != NULL) { // ���ϵ�ɾ����һ�����
            nodeTable[i].adj = p->link;
            delete p;
            p = nodeTable[i].adj;
        }
    }
    //delete nodeTable; // ɾ�����������
}

SequeGraph& SequeGraph::operator=(SequeGraph& Graph)
{
    this->maxVertices = Graph.maxVertices;
    this->numVertices = Graph.numVertices;
    //nodeTable = new Vertex[maxVertices]; // �������������
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

// �����ڽӱ��ʾ��ͼ
void SequeGraph::inputGraph() {

    int n, m;                                // �洢�������ͱ���
    int work, time;                     // ��ҵ�ڵ�
    char direction;
    int e1, e2;                             // ��
    int i, j, k;

    cout << "�����붥�����ͱ�����" << endl;
    cin >> n >> m;

    cout << "���������ҵ��ź�ʱ�䣺" << endl;
    for (i = 0; i < n; i++) {
        cin >> work >> time >> direction;
        insertVertex(work, time, direction);           // ���붥��
    }

    cout << "������ͼ�ĸ��ߵ���Ϣ��" << endl;
    i = 0;
    while (i < m) {
        cin >> e1 >> e2;
        j = getVertexPos(e1);
        k = getVertexPos(e2);
        if (j == -1 || k == -1)
            cout << "�����˵���Ϣ�������������룡" << endl;
        else {
            insertEdge(e1, e2); // �����
            i++;
        }
    } // while
}

// �������ͼ�е����ж���ͱ���Ϣ
void SequeGraph::outputGraph() {
    int n, m, i;
    int e1, e2;                                 // ����
    Edge* p;

    n = numVertices;
    m = numEdges;
    cout << "ͼ�еĶ�����Ϊ" << n << ",����Ϊ" << m << endl;
    for (i = 0; i < n; i++) {
        p = nodeTable[i].adj;
        while (p != NULL) {
            e1 = getValue(i); // �����<i, p->dest>
            e2 = getValue(p->dest);
            cout << "<" << e1 << ", " << e2  << ">" << endl;
            p = p->link; // ָ����һ���ڽӶ���
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

// ȡλ��Ϊi�Ķ����е���ҵ���
int SequeGraph::getValue(int i) {
    if (i >= 0 && i < numVertices)
        return nodeTable[i].work;
    return NULL;
}

// ���붥��
bool SequeGraph::insertVertex(const int work, const int time , const char direction) {
    if (numVertices == maxVertices)                   // ������������ܲ���
    {
        cout << "the vertex table fulled!" << endl;
        return false;
    }
    else
    {
        nodeTable[numVertices].work = work;       // �����ڱ�����
        nodeTable[numVertices].time = time;
        nodeTable[numVertices].direction = direction;
        nodeTable[numVertices].inDegree = 0;
        nodeTable[numVertices].outDegree = 0;
        nodeTable[numVertices].setupTime = 0;
        numVertices++;
    }

    return true;
}

// �����
bool SequeGraph::insertEdge(int work1, int work2) {
    int v1 = getVertexPos(work1);
    int v2 = getVertexPos(work2);
    nodeTable[v1].outDegree++;
    nodeTable[v2].inDegree++;
    if (v1 >= 0 && v1 < numVertices && v2 >= 0 && v2 < numVertices) {
        Edge* p = nodeTable[v1].adj;                    // v1��Ӧ�ı�����ͷָ��
        while (p != NULL && p->dest != v2)       // Ѱ���ڽӶ���v2
            p = p->link;
        if (p != NULL)                                           // �Ѵ��ڸñߣ�������
        {
            cout << "this edge already existed!";
            return false;
        }
        p = new Edge();                                             // �����±߽��
        p->dest = v2;
        p->link = nodeTable[v1].adj;                      // ����v1��������λ
        nodeTable[v1].adj = p;
        numEdges++;
        return true;
    }
    return false;
}

// ����ͼɾ��������鷳
bool SequeGraph::removeVertex(int work) {
    int v = getVertexPos(work);
    if (numVertices < 1 || v < 0 || v > numVertices)
        return false; // ��ջ򶥵�ų�����Χ

    Edge* p, * s;

    // 1.�������v�ı�������w ��<v,w>����delete �� v Ϊ���ıߣ�
    while (nodeTable[v].adj != NULL) 
    {
        p = nodeTable[v].adj;
        nodeTable[v].adj = p->link;
        if(p->dest>=0 && p->dest < numVertices)
            nodeTable[p->dest].inDegree--;
        nodeTable[v].outDegree--;
        delete p;
        numEdges--; // �붥��v������ı�����1
    } // while����

    // 2.���<w, v>����v�йصıߣ��� delete �� v Ϊ�յ�ıߣ�
    for (int i = 0; i < numVertices; i++) {
        if (i != v)
        { // ���ǵ�ǰ����v
            s = NULL;
            p = nodeTable[i].adj;
            while (p != NULL && p->dest != v) 
            {                                       // �ڶ���i����������v�Ķ���
                s = p;
                p = p->link;                 // ������
            }
            if (p != NULL) {             // �ҵ���v�Ľ��
                if (s == NULL)           // ˵��p��nodeTable[i].adj
                {         
                    nodeTable[i].adj = p->link;
                }
                else {
                    s->link = p->link; // ����p����һ��������Ϣ
                }
                nodeTable[i].outDegree--;
                nodeTable[v].inDegree--;
                delete p; // ɾ�����p
                numEdges--; // �붥��v������ı�����1
            }
        }
    }
    numVertices--; // ͼ�Ķ��������1
    
    //3.��β��numVertics�����ɾ����λ��
    nodeTable[v].work = nodeTable[numVertices].work; // ��ʱnumVertices����ԭ��numVerticesС1�����ԣ����ﲻ��ҪnumVertices-1
    nodeTable[v].time = nodeTable[numVertices].time;
    nodeTable[v].direction = nodeTable[numVertices].direction;
    nodeTable[v].inDegree = nodeTable[numVertices].inDegree;
    nodeTable[v].outDegree = nodeTable[numVertices].outDegree;
    nodeTable[v].FrontVertexs = nodeTable[numVertices].FrontVertexs;
    nodeTable[v].NextVertexs = nodeTable[numVertices].NextVertexs;
    nodeTable[v].adj = nodeTable[numVertices].adj;

    // 4.Ҫ����Ķ����Ӧ��λ�ø�д
    for (int i = 0; i < numVertices; i++) {
        p = nodeTable[i].adj;
        while (p != NULL && p->dest != numVertices) // �ڶ���i����������numVertices�Ķ���
            p = p->link;                                                       // ������
        if (p != NULL)                                                      // �ҵ���numVertices�Ľ��
            p->dest = v;                                                       // ���ڽӶ���numVertices�ĳ�v
    }

    //5.����������λ��numVertics�Ķ�������
    nodeTable[numVertices].work = 0;
    nodeTable[numVertices].time = 0;
    nodeTable[numVertices].inDegree = 0;
    nodeTable[numVertices].outDegree = 0;
    nodeTable[numVertices].direction = NULL;

    return true;
}

// ɾ����
bool SequeGraph::removeEdge(int work1, int work2) {
    int v1 = getVertexPos(work1);
    int v2 = getVertexPos(work2);
    if (v1 != -1 && v2 != -1) {
        Edge* p = nodeTable[v1].adj, * q = NULL;
        while (p != NULL && p->dest != v2) { // v1��Ӧ���������ұ�ɾ����
            q = p;
            p = p->link;
        }
        if (p != NULL) { // �ҵ���ɾ���߽��
            if (q == NULL) // ɾ���Ľ���Ǳ�������׽��
                nodeTable[v1].adj = p->link;
            else
                q->link = p->link; // ���ǣ���������
            delete p;
            return true;
        }
    }
    return false; // û���ҵ����
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

// ȡ����v�ĵ�һ���ڽӶ���
int SequeGraph::getFirstNeighbor(int work) {
    int v = getVertexPos(work);
    if (v != -1) {
        Edge* p = nodeTable[v].adj; // ��Ӧ�����һ���߽��
        if (p != NULL) // ���ڣ����ص�һ���ڽӶ���
            return p->dest;
    }
    return -1; // ��һ���ڽӶ��㲻����
}

// ȡ����v���ڽӶ���w����һ�ڽӶ���
int SequeGraph::getNextNeighbor(int v, int w)
{
    if (v != -1) {
        Edge* p = nodeTable[v].adj;                 // ��Ӧ�����һ���߽��
        while (p != NULL && p->dest != w)   // Ѱ���ڽӶ���w
            p = p->link;
        if (p != NULL && p->link != NULL)
            return p->link->dest;              // ������һ���ڽӶ���
    }
    return -1;                                         // ��һ���ڽӶ��㲻����
}

// ��������vertex��ͼ�е�λ��
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

// ��ǰ������
int SequeGraph::numberOfVertices() {
    return numVertices;
}

set<int>& SequeGraph::getFrontVertexs(const int work)
{
    int v = getVertexPos(work);
    Edge *p;
    for (int i = 0; i < numVertices; i++)
    {
        if (i != v)                              // ���ǵ�ǰ����v
        {
            p = nodeTable[i].adj;
            while (p != NULL && p->dest != v)
            {                                       // �ڶ���i����������v�Ķ���
                p = p->link;                 // ������
            }
            if (p != NULL)               // �ҵ���v�Ľ��( p->dest == v )
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