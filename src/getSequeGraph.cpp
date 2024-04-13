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
    SequeGraph st; // ��������
    bool finished = 0;
    int choice;
    char e1, e2, next;
    int work, time;
    char direction;

    while (!finished) {
        cout << "[1]���������ڽӱ������ͼ" << endl;
        cout << "[2]���ͼ�����ж���ͱ���Ϣ" << endl;
        cout << "[3]ȡ����v�ĵ�һ���ڽӶ���" << endl;
        cout << "[4]ȡv���ڽӶ���w����һ���ڽӶ���" << endl;
        cout << "[5]���붥�㣨��ҵ��ţ���ҵʱ�䣩" << endl;
        cout << "[6]����ߣ������ţ��յ���ţ�" << endl;
        cout << "[7]ɾ�����㣨�����ţ�" << endl;
        cout << "[8]ɾ���ߣ������ţ��յ���ţ�" << endl;
        cout << "[9]��ӡ�ڽӱ�" << endl;
        cout << "[10]�˳�" << endl;
        cout << "������ѡ��[1-10]��";
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
            cout << "�����붥��:";
            cin >> e1;
            e2 = st.getValue(st.getFirstNeighbor(st.getVertexPos(e1)));
            if (e2)
                cout << "����" << e1 << "�ĵ�һ���ڽӶ���Ϊ:" << e2 << endl;
            else
                cout << "����" << e1 << "û���ڽӶ��㣡" << endl;
            break;
        case 4:
            cout << "�����붥��v���ڽӶ���w:";
            cin >> e1 >> e2;
            next = st.getValue(st.getNextNeighbor(st.getVertexPos(e1), st.getVertexPos(e2)));
            if (next)
                cout << "����" << e1 << "���ڽӶ���" << e2 << "����һ���ڽӶ���Ϊ:" << next << endl;
            else
                cout << "����" << e1 << "���ڽӶ���" << e2 << "û����һ���ڽӶ��㣡" << endl;
            break;
        case 5:
            cout << "������Ҫ����Ķ��㣺��ҵ��ţ���ҵʱ�䣬������λ";
            cin >> work >> time >> direction;
            if (st.insertVertex(work, time, direction))
                cout << "����ɹ���" << endl;
            else
                cout << "������������ʧ�ܣ�" << endl;
            break;
        case 6:
            cout << "������Ҫ����ıߵ���Ϣ:" << endl;
            cin >> e1 >> e2;
            st.insertEdge(st.getVertexPos(e1), st.getVertexPos(e2));
            break;
        case 7:
            cout << "������Ҫɾ���Ķ��㣺";
            cin >> e1;
            if (st.removeVertex(st.getVertexPos(e1)))
                cout << "����" << e1 << "��ɾ��!" << endl;
            else
                cout << "����" << e1 << "����ͼ��!" << endl;
            break;
        case 8:
            cout << "������Ҫɾ���ıߵ������˵㣺" << endl;
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
            cout << "ѡ�������������������!" << endl;
        }
    }
}