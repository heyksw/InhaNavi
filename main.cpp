/* 
12161540 ���� 

< �˰��� ���� ���� 2 : ������̼� ���α׷� ���� >

- ������ ��� 
(1) ������ �Է¹ޱ�
(2) �� ���� ���� �ִܰŸ� ã�� A S D
(3) �� ���� ���� �ִܰ�� ã�� B S D

*/
#include <stdio.h>
#include <iostream>
#include <vector>
#include <queue>
#include <string>
using namespace std;
#define INF 1e6	// 10^6 �� �ʰ��ϸ� �������� ����

// Graph�� Node = ����
class Node {
public:
	bool isNull;	// ��尡 totalNode�� ������ false, ������ true.
	int num;	// ������ȣ
	string name;	// �����̸�
	bool isFlood;	// ħ������

	Node() {
		this->isNull = false;
		this->num = 0;
		this->name = "null";
		this->isFlood = false;
	}

	Node(int _num, string _name, bool _isFlood) {
		this->isNull = true;
		this->name = _name;
		this->num = _num;
		this->isFlood = _isFlood;
	}
};

// ���ڿ� �Ľ� �Լ�
vector<string> parsing(string str) {
	int previous = 0;
	int current = 0;
	vector<string> x;
	x.clear();

	current = str.find(' ');
	//find�� ã���� ������ npos�� ������
	while (current != string::npos) {
		string substring = str.substr(previous, current - previous);
		x.push_back(substring);
		previous = current + 1;
		current = str.find(' ', previous);
	}
	x.push_back(str.substr(previous, current - previous));

	/*
	for (int i = 0; i < x.size(); i++) {
		cout << x[i] << " ";
	}
	cout << endl;
	*/
	return x;
}


Node totalNode[1000000];	// ��ü ��带 ��� �迭
// graph[n] ���� ���� ��ȣ n�� ��忡 ����� ������ ��������Ʈ ������ ����Ǿ��ִ�.
// pair<Node, int> -> Node : ���� ���, int : �Ÿ�
vector< pair<Node, int> > graph[1000000];	// ��� ��ȣ�� 100,000 ~ 999,999 ������ ����.
int d[1000000];	// �ִܰŸ� ���̺�

// N�� trace�� ���ÿ� return ���ֱ����� ���� ����ü.
struct myStruct {
	int N;	// tree vertice ��
	vector<int>* trace;	// �ִܰŸ��� ������Ʈ �ɶ� �ٷ� ������ �湮�ϰ� �Ǵ� ����
};

class Navi {
private:	
	// n: �������� ��, m: �������� ��, q: ���� ��
	int n = 0;
	int m = 0;
	int q = 0;	
	int num = 0;


public:
	
	void insertNode(int _num, string _name, bool _isFlood) {	// ��� �Է�
		Node temp = Node(_num, _name, _isFlood);
		totalNode[_num] = temp;
	}

	void insertEdge(int _num1, int _num2, int _distance) {	// ���� �Է�
		graph[_num1].push_back(pair<Node, int>(totalNode[_num2], _distance));
		graph[_num2].push_back(pair<Node, int>(totalNode[_num1], _distance));
	}

	// �켱���� ť�� �̿��� ���ͽ�Ʈ��
	myStruct Dijkstra(int start) {	// ���ͽ�Ʈ�� �˰���
		myStruct result;
		vector<int>* from = new vector<int>(1000000);

		int N=0;	// tree vertice ��

		priority_queue< pair<int, int> > pq;	// �Ÿ�, ����ȣ

		pq.push({ 0, start });
		N++;
		d[start] = 0;

		while (!pq.empty()) {
			int dist = -pq.top().first;	// ���� �������� ���
			int now = pq.top().second;	// ���� ���
			// cout << "dist, now : " << dist << now << endl;
			pq.pop();

			if (d[now] < dist)	// �̹� �ִܰ�� ó���� ����� ��� �н�
				continue;

			for (int i = 0; i < graph[now].size(); i++) {

				if (graph[now][i].first.isFlood) {	// ħ���Ǿ��� ��� ����.
					continue;
				}

				// cout << graph[now][i].first.num << "�� ���� d �� : " << d[graph[now][i].first.num] << endl;
				// �� ��带 ���İ��� ��� ���
				int cost = dist + graph[now][i].second;
				// ����� �� �۴ٸ� �ִܰ�� ���̺� ������Ʈ
				if (cost < d[graph[now][i].first.num]) {
					d[graph[now][i].first.num] = cost;
					pq.push(make_pair(-cost, graph[now][i].first.num));
					N++;
					//cout << "1" << endl;
					(*from)[graph[now][i].first.num] = now;
				}
				else {
					//cout << "2" << endl;
				}

			}

		}

		result = { N,from };

		return result;
	}

	void trace_path(int s, int e, vector<int>* from) {

		if ((*from)[e] == s) {
			cout << totalNode[s].name << " -> ";
			return;

		}

		// ���ȣ���� ���� ���� e���� ������ ���� ��θ� ����Ѵ�..  
		trace_path(s, (*from)[e], from);

		// �ִܰ�ο��� ���� e �ٷ� ������ ������ ȭ�鿡 ����Ѵ�.  
		cout << totalNode[(*from)[e]].name << " - > ";
	}
	
	// ������ �������� �����ϰ� ȭ���� ����ϱ� ���� �Լ� �и�  
	void print_path(int s, int e, vector<int>* from) {
		// ���� trace_path�� ȣ���Ͽ� �ִ� ��θ� �������,  
		trace_path(s, e, from);

		// �������� ���� ��ȣ�� ����Ѵ�.  
		cout << totalNode[e].name;
	}

	void shortestDistance(int S, int D) {	// �ִܰŸ� ã��
		myStruct s = Dijkstra(S);
		if(d[D] >= INF){
			cout << "None" << endl;
		}
		else {
			cout << "�ִܰŸ� : " << d[D] << " " << totalNode[S].name << " " << totalNode[D].name << endl;
		}
	}

	void shortestPath(int S, int D) {	// �ִܰ�� ã��
		myStruct s = Dijkstra(S);
		if (d[D] >= INF) {
			cout << "None" << endl;
		}
		else {
			//cout << s.N << " ";
			print_path(S, D, s.trace);
		}
	}

	
	// main program
	void program() {
		cout << " ===== Inha Navigation Program =====" << endl;
		cout << "					by KSW" << endl << endl;

		int n, m, q;	
		cout << "���� ���� ��, ���� ���� ��, ������ ���� �Է��ϼ���." << endl;
		cin >> n >> m >> q;

		this->n = n;
		this->m = m;
		this->q = q;

		for (int i = 0; i < n; i++) {	// �������� �Է¹ޱ�
			cout << i + 1 << " ��° �������� : " << endl;
			int a, c;
			string b;
			cin >> a >> b >> c;
			insertNode(a, b, c);
		}

		for (int i = 0; i < m; i++) {	// �������� �Է¹ޱ�
			cout << i + 1 << " ��° �������� :" << endl;
			int a, b, c;
			cin >> a >> b >> c;
			insertEdge(a, b, c);
		}



		/*cout << "üũ�� ��ȣ num:" << endl;
		int num;
		cin >> num;
		Dijkstra(num);
		cout << "���ͽ�Ʈ�� ����. graph[num]���� " << endl;
		for (int i = 0; i < graph[num].size(); i++) {
			cout << graph[num][i].first.num << " ������ �Ÿ��� "
				<< d[graph[num][i].first.num] << endl;
		}
		cout << endl;*/

		cin.ignore();

		for (int i = 0; i < q; i++) {	// ���� �Է¹ޱ�
			// �ִܰŸ� ���̺� �ʱ�ȭ
			fill(d, d + 1000000, INF);

			cout << i + 1 << " ��° ���� : " << endl;
			string cmd;
			getline(cin, cmd);
			vector<string> cmdVector = parsing(cmd);
			
			if (cmdVector[0] == "A") {	// �ִܰŸ� ã��
				//cout << "A�� �Է��߽��ϴ�" << endl;
				//cout << cmdVector[1] << " ���� " << cmdVector[2] << "������ �ִܰŸ� : ";
				shortestDistance(stoi(cmdVector[1]), stoi(cmdVector[2]));

			}
			else if (cmdVector[0] == "B") {	// �ִܰ�� ã��
				//cout << "B�� �Է��߽��ϴ�" << endl;
				cout << " == �ִ� ��θ� Ž���մϴ� == " << endl;
				cout << "��� : ";
				shortestPath(stoi(cmdVector[1]), stoi(cmdVector[2]));
				cout << endl << endl;
			}

		}

	}
};


int main() {
	Navi InhaNavi;
	InhaNavi.program();

	return 0;
}

