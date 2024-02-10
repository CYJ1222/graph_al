#pragma once
#include "header.h"
#include "seqStack.h"
#include "seqQueue.h"
using namespace std;

template <class verType, class edgeType>
class verNode;

template <class verType, class edgeType>
class Graph;

template <class verType, class edgeType>
class edgeNode
{
    friend class verNode<verType, edgeType>;
    friend class Graph<verType, edgeType>;

private:
    verNode<verType, edgeType> *dest; // 射入的节点的指针
    edgeType weight;                  // 边的权重
    edgeNode *next;                   // 单链表中的*next
    bool visited = false;             // 记录边是否访问过,用于Euler circle/path
public:
    edgeNode(verNode<verType, edgeType> *destination = nullptr, edgeType edgeWeight = 1, edgeNode *nextNode = nullptr) : dest(destination), weight(edgeWeight), next(nextNode) {}
};

template <class verType, class edgeType>
class verNode
{
    friend class edgeNode<verType, edgeType>;
    friend class Graph<verType, edgeType>;

private:
    verType ver;                       // vertex的值（不一定是下标）
    edgeNode<verType, edgeType> *head; // 邻接单链表
    verNode *next;                     // 下一个顶点
    int outdegree;//出度
    int indegree;//入度
    bool visited;              // 记录节点是否被访问过，用于BFS DFS
public:
    verNode() : next(nullptr) 
    { 
        head = new edgeNode<verType, edgeType>(); 
        outdegree=0;
        indegree=0;
        visited= false;
    }
    verNode(verType vertex, verNode *nextVertex = nullptr) : ver(vertex), next(nextVertex) 
    { 
        head = new edgeNode<verType, edgeType>(); 
        outdegree=0;
        indegree=0;
        visited= false;
    }
    // 因为边链表删除与边的数目有关，所以在graph函数中实现
    ~verNode()
    {
        if (head->next)
        {
            cerr<<"memory leak";
        }
        delete head;
    }
};

template <class verType, class edgeType>
class Graph
{
private:
    bool directed;                    // 有向图为1，无向图为0
    int verts, edges;                 // 图的实际顶点数和实际边数
    verNode<verType, edgeType> *head; // 保存顶点数据的一维数组
    // 返回边的指针
    edgeNode<verType, edgeType> *getEdge(const verType &vertex1, const verType &vertex2) const;
    // 返回顶点的指针
    verNode<verType, edgeType> *getVertex(const verType &vertex) const;
    // 深度优先搜索算法递归调用部分
    void DFS(verNode<verType, edgeType> *start) const;
    // 判断有向图连通性
    bool connected_dircted() const;
    // 判断无向图连通性
    bool connected_undircted() const;

public:
    // 初始化图结构g，direct为是否有向图标志,true为有向图,false为无向图
    Graph(bool direct = false);
    // 返回图当前顶点数
    int numberOfVertex() const { return verts; };
    // 返回图当前边数
    int numberOfEdge() const { return edges; };
    // 插入顶点
    void insertVertex(verType vertex);
    // 插入边
    void insertEdge(const verType &vertex1, const verType &vertex2, edgeType edge = 1);
    // 删除顶点
    void removeVertex(verType vertex);
    // 删除边
    void removeEdge(verType vertex1, verType vertex2);
    // 显示邻接表
    void display() const;
    // 深度优先搜索递归实现,接近head的边节点先访问
    void DFS_R() const;
    // 深度优先搜索动态规划实现，接近head的边节点后访问
    void DFS_D() const;
    // 广度优先搜索
    void BFS() const;
    // 图的连通性，连通返回true,
    bool connected() const;
    //应该用指针传递避免对象被销毁
    Graph clone() const;
    ~Graph();
};
template <class verType, class edgeType>
edgeNode<verType, edgeType> *Graph<verType, edgeType>::getEdge(const verType &vertex1, const verType &vertex2) const
{
    verNode<verType, edgeType> *p = getVertex(vertex1);
    if (!p)
        throw invalid_argument("vertex1 unexist");
    edgeNode<verType, edgeType> *q = p->head->next;
    while (q)
    {
        if (q->dest->ver == vertex2)
            return q;
        q = q->next;
    }
    return nullptr;
}
template <class verType, class edgeType>
verNode<verType, edgeType> *Graph<verType, edgeType>::getVertex(const verType &vertex) const
{
    verNode<verType, edgeType> *p = head->next;
    while (p)
    {
        if (p->ver == vertex)
            return p;
        p = p->next;
    }
    return nullptr;
}
// 深度优先搜索算法递归调用部分
template <class verType, class edgeType>
void Graph<verType, edgeType>::DFS(verNode<verType, edgeType> *start) const
{
    // 断言 start->visited为false，否则DFS()函数出错
    assert(!start->visited);
    edgeNode<verType, edgeType> *p;
    // 输出该节点信息
    cout << start->ver << ' ';
    start->visited = true;
    p = start->head->next;
    // 遍历边链表
    while (p)
    {
        verNode<verType, edgeType> *v = p->dest;
        // 递归
        if (!v->visited)
            DFS(v);
        p = p->next;
    }
}

// 判断有向图连通性
template <class verType, class edgeType>
bool Graph<verType, edgeType>::connected_dircted() const
{
    cerr<<"function have'n completed";
    return true;
}
// 判断无向图连通性
template <class verType, class edgeType>
bool Graph<verType, edgeType>::connected_undircted() const
{
    seqQueue<verNode<verType, edgeType> *> q;
    edgeNode<verType, edgeType> *p;
    verNode<verType, edgeType> *v = head->next, *start;
    int count = 0; // count为计数器

    // 置初始访问标志为false。
    while (v)
    {
        v->visited = false;
        v = v->next;
    }
    // 逐一找到未被访问过顶点，
    // 做广度优先遍历
    v = head->next;
    while (v)
    {
        if (v->visited)
        {
            v = v->next;
            continue;
        }
        q.enQueue(v);
        count++;
        while (!q.isEmpty())
        {
            start = q.getFront();
            q.deQueue();
            if (start->visited)
                continue;
            start->visited = true;
            p = start->head->next;
            while (p)
            {
                if (!p->dest->visited)
                    q.enQueue(p->dest);
                p = p->next;
            }
        }
        cout << '\n';
    }
    if (count == 1)
        return true;
    return false;
}

// 初始化图结构g，direct为是否有向图标志
template <class verType, class edgeType>
Graph<verType, edgeType>::Graph(bool direct)
{
    // 初始化属性
    directed = direct;
    verts = 0;
    edges = 0;
    // 为存顶点的一维数组创建空间
    head = new verNode<verType, edgeType>();
}
// 插入顶点
template <class verType, class edgeType>
void Graph<verType, edgeType>::insertVertex(verType vertex)
{
    if (getVertex(vertex))
        throw invalid_argument("same vertex has exist");
    head->next = new verNode<verType, edgeType>(vertex, head->next);
    verts++;
}
// 插入边
template <class verType, class edgeType>
void Graph<verType, edgeType>::insertEdge(const verType &vertex1, const verType &vertex2, edgeType edge)
{
    if (getEdge(vertex1, vertex2))
        throw invalid_argument("edge has exist");
    // 找到vertex1节点 vertex2节点
    verNode<verType, edgeType> *p = getVertex(vertex1), *q = getVertex(vertex2);
    // vertex1不存在
    if (!p)
        throw invalid_argument("unexist vertex1");
    // vertex2不存在
    if (!q)
        throw invalid_argument("unexist vertex2");
    // 加入边<vertex1,vertex2>

    p->head->next = new edgeNode<verType, edgeType>(q, edge, p->head->next);
    p->outdegree++;
    q->indegree++;
    edges++;
    if (!directed) // 无向图，加入边<vertex2,vertex1>
    {
        q->head->next = new edgeNode<verType, edgeType>(p, edge, q->head->next);
        q->outdegree++;
        p->indegree++;
        edges++;
    }
}

// 删除顶点
template <class verType, class edgeType>
void Graph<verType, edgeType>::removeVertex(verType vertex)
{
    verNode<verType, edgeType> *v = head->next, *tmpV = head->next, *f = head; // f记录v前面的节点
    int count = 0;
    edgeNode<verType, edgeType> *p, *q;
    // 找到该顶点在顶点表中的节点
    while (v)
    {
        if (v->ver == vertex)
            break;
        f = v;
        v = v->next;
    }

    if (!v)
        throw invalid_argument("vertex does not exist");
    // 删除顶点引导的单链表中所有结点并计数删除的边
    //不用记录该节点度的变化，但是要记录其目标节点的入度变化
    p = v->head->next;
    while (p)
    {
        count++;
        v->head->next = p->next;
        p->dest->indegree--;
        delete p;
        p = v->head->next;
    }
    // 检查所有单链表，凡是dest是vertex的都删除
    while (tmpV)
    {
        p = tmpV->head->next;
        q = tmpV->head;
        // 遍历边链表，寻找vertex,不调用getEdge()是因为要看前一个边节点
        while (p)
        {
            if (p->dest == v)
                break;
            q = p;
            p = p->next;
        }
        // 该边链表无vertex,跳到下个顶点
        if (!p)
        {
            tmpV = tmpV->next;
            continue;
        }
        // 删除dest为vertex的边
        q->next = p->next;
        p->dest->outdegree--;
        delete p;
        count++;
        tmpV = tmpV->next;
    }
    // 在顶点表中删除顶点
    f->next = v->next;
    delete v;
    verts--;
    // 有向图，
    if (directed)
        edges -= count;
    // 无向图，减少count的一半
    else
        edges -= count / 2;
}

// 删除边
template <class verType, class edgeType>
void Graph<verType, edgeType>::removeEdge(verType vertex1, verType vertex2)
{
    verNode<verType, edgeType> *p = getVertex(vertex1), *q = getVertex(vertex2);
    edgeNode<verType, edgeType> *tmp, *f;
    // vertex1不存在
    if (!p)
        throw invalid_argument("unexist vertex1");
    // vertex2不存在
    if (!q)
        throw invalid_argument("unexist vertex2");
    // 删除边<vertex1,vertex2>
    tmp = p->head->next;
    f = p->head;
    // 找到边<vertex1 vertex2>
    while (tmp)
    {
        if (tmp->dest == q)
            break;
        f = tmp;
        tmp = tmp->next;
    }
    // 边不存在
    if (!tmp)
        throw invalid_argument("<vertex1 vertex2> not exist");
    // 删除边
    f->next = tmp->next;
    p->outdegree--;
    q->indegree--;
    delete tmp;
    edges--;
    // 无向图，删除边<vertex2,vertex1>
    if (!directed)
    {
        // 删除边<vertex2,vertex1>
        tmp = q->head->next;
        f = q->head;
        // 找到边<vertex2 vertex1>
        while (tmp)
        {
            if (tmp->dest == p)
                break;
            f = tmp;
            tmp = tmp->next;
        }
        // 边不存在
        if (!tmp)
            throw invalid_argument("<vertex2 vertex1> not exist");
        // 删除边
        f->next = tmp->next;
        q->outdegree--;
        p->indegree--;
        delete tmp;
        edges--;
    }
}

// 显示邻接表
template <class verType, class edgeType>
void Graph<verType, edgeType>::display() const
{
    verNode<verType, edgeType> *v = head->next;
    edgeNode<verType, edgeType> *q;
    // 遍历节点单链表
    while (v)
    {
        cout << v->ver;
        q = v->head->next;
        // 遍历节点的边链表
        while (q)
        {
            cout << ' ' << q->dest->ver;
            q = q->next;
        }
        cout << '\n';
        v = v->next;
    }
}

// 深度优先搜索递归实现
template <class verType, class edgeType>
void Graph<verType, edgeType>::DFS_R() const
{
    verNode<verType, edgeType> *v = head->next;
    // 遍历节点链表，置初始访问标志为false
    while (v)
    {
        v->visited = false;
        v = v->next;
    }
    v = head->next;
    // 遍历节点链表，队每个！visited的v调用DFS(v)
    while (v)
    {
        if (!v->visited)
        {
            DFS(v);
            cout << endl;
        }

        v = v->next;
    }
}
// 深度优先搜索动态规划实现
template <class verType, class edgeType>
void Graph<verType, edgeType>::DFS_D() const
{
    seqStack<verNode<verType, edgeType> *> s;
    edgeNode<verType, edgeType> *p;
    verNode<verType, edgeType> *v = head->next, *start;
    // 置初始访问标志为false。
    while (v)
    {
        v->visited = false;
        v = v->next;
    }
    v = head->next;
    // 逐一找到未被访问过顶点，做深度优先遍历
    while (v)
    {
        if (v->visited)
        {
            v = v->next;
            continue;
        }
        s.push(v);
        // 利用stack做DFS
        while (!s.isEmpty())
        {
            start = s.top();
            s.pop();
            if (start->visited)
                continue;
            cout << start->ver << ' ';
            start->visited = true;

            p = start->head->next;
            // 每个边节点都要把dest的所有！visieted结点push
            while (p)
            {
                verNode<verType, edgeType> *tmp = p->dest;
                if (!tmp->visited)
                    s.push(tmp);
                p = p->next;
            }
        }
        cout << endl;
    }
}
// 广度优先遍历
template <class verType, class edgeType>
void Graph<verType, edgeType>::BFS() const
{
    seqQueue<verNode<verType, edgeType> *> q;
    edgeNode<verType, edgeType> *p;
    verNode<verType, edgeType> *v = head->next, *start;
    // 置初始访问标志为false。
    while (v)
    {
        v->visited = false;
        v = v->next;
    }
    // 逐一找到未被访问过顶点，
    // 做广度优先遍历
    v = head->next;
    while (v)
    {
        if (v->visited)
        {
            v = v->next;
            continue;
        }
        q.enQueue(v);
        while (!q.isEmpty())
        {
            start = q.getFront();
            q.deQueue();
            if (start->visited)
                continue;
            cout << start->ver << ' ';
            start->visited = true;
            p = start->head->next;
            while (p)
            {
                if (!p->dest->visited)
                    q.enQueue(p->dest);
                p = p->next;
            }
        }
        cout << '\n';
    }
}
// 图的连通性，连通返回true
template <class verType, class edgeType>
bool Graph<verType, edgeType>::connected() const
{
    if (directed)
        return connected_dircted();
    return connected_undircted();
}
template <class verType, class edgeType>
Graph<verType, edgeType>::~Graph()
{
    verNode<verType, edgeType> *v = head->next;
    edgeNode<verType, edgeType> *e;
    // 释放所有边表中的结点
    while (v)
    {
        e = v->head->next;
        while (e)
        {
            v->head->next = e->next;
            delete e;
            e = v->head->next;
        }
        head->next = v->next;
        delete v;
        v = head->next;
    }
    delete head;
}