#include "SequeGraph.h"

void test();
void getSeGraph();

void test()
{
    SequeGraph te(10);
    for (int i = 1; i <= 10; i++)
    {
        te.insertVertex(i, 10 - i, 'E');
        if (i >1)
        {
            if(!te.insertEdge(i, i -1))
                cout<<"failed to insert Edge"<<endl;
        }
    }
    for (int i = 1; i <= 8; i++)
    {
        if (!te.insertEdge(i, i + 2))
            cout << "failed to insert Edge!" << endl;
    }

    cout << "The nextVertexs set:" << endl;
    for (int i = 0; i < 10; i++)
    {
        set<int> temp = te.getNextVertexs(te.getValue(i));
        for (auto x : temp)
        {
            cout << x << " ";
        }
        cout << endl;
    }

    cout << "The frontVertexs set:" << endl;
    for (int i = 0; i < 10; i++)
    {
        set<int> temp = te.getFrontVertexs(te.getValue(i));
        for (auto x : temp)
        {
            cout << x << " ";
        }
        cout << endl;
    }

    te.outputGraph();
    te.printself();
}

void getSeGraph()
{
    SequeGraph st; // 声明对象
    bool finished = 0;
    int choice;
    char e1, e2, next;
    int work, time;
    char direction;

    while (!finished) {
        cout << "[1]创建基于邻接表的有向图" << endl;
        cout << "[2]输出图的所有顶点和边信息" << endl;
        cout << "[3]取顶点v的第一个邻接顶点" << endl;
        cout << "[4]取v的邻接顶点w的下一个邻接顶点" << endl;
        cout << "[5]插入顶点（作业序号，作业时间）" << endl;
        cout << "[6]插入边（起点序号，终点序号）" << endl;
        cout << "[7]删除顶点（起点序号）" << endl;
        cout << "[8]删除边（起点序号，终点序号）" << endl;
        cout << "[9]打印邻接表" << endl;
        cout << "[10]退出" << endl;
        cout << "请输入选择[1-10]：";
        cin >> choice;
        switch (choice) {
        case 1:
            st.inputGraph();
            cout << endl;
            break;
        case 2:
            st.outputGraph();
            cout << endl;
            break;
        case 3:
            cout << "请输入顶点:";
            cin >> e1;
            e2 = st.getValue(st.getFirstNeighbor(st.getVertexPos(e1)));
            if (e2)
                cout << "顶点" << e1 << "的第一个邻接顶点为:" << e2 << endl;
            else
                cout << "顶点" << e1 << "没有邻接顶点！" << endl;
            break;
        case 4:
            cout << "请输入顶点v和邻接顶点w:";
            cin >> e1 >> e2;
            next = st.getValue(st.getNextNeighbor(st.getVertexPos(e1), st.getVertexPos(e2)));
            if (next)
                cout << "顶点" << e1 << "的邻接顶点" << e2 << "的下一个邻接顶点为:" << next << endl;
            else
                cout << "顶点" << e1 << "的邻接顶点" << e2 << "没有下一个邻接顶点！" << endl;
            break;
        case 5:
            cout << "请输入要插入的顶点：作业序号，作业时间，操作方位";
            cin >> work >> time >> direction;
            if (st.insertVertex(work, time, direction))
                cout << "插入成功！" << endl;
            else
                cout << "表已满，插入失败！" << endl;
            break;
        case 6:
            cout << "请输入要插入的边的信息:" << endl;
            cin >> e1 >> e2;
            st.insertEdge(st.getVertexPos(e1), st.getVertexPos(e2));
            break;
        case 7:
            cout << "请输入要删除的顶点：";
            cin >> e1;
            if (st.removeVertex(st.getVertexPos(e1)))
                cout << "顶点" << e1 << "已删除!" << endl;
            else
                cout << "顶点" << e1 << "不在图中!" << endl;
            break;
        case 8:
            cout << "请输入要删除的边的两个端点：" << endl;
            cin >> e1 >> e2;
            st.removeEdge(st.getVertexPos(e1), st.getVertexPos(e2));
            break;
        case 9:
            st.printself();
            break;
        case 10:
            finished = true;
            break;
        default:
            cout << "选择输入错误，请重新输入!" << endl;
        }
    }
}