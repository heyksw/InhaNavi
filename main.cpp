/* 
12161540 김상우 

< 알고리즘 설계 과제 2 : 내비게이션 프로그램 설계 >

- 수행할 기능 
(1) 데이터 입력받기
(2) 두 지역 간의 최단거리 찾기 A S D
(3) 두 지역 간의 최단경로 찾기 B S D

*/
#include <stdio.h>
#include <iostream>
#include <vector>
#include <queue>
#include <string>
using namespace std;
#define INF 1e6	// 10^6 을 초과하면 무한으로 간주

// Graph의 Node = 지역
class Node {
public:
	bool isNull;	// 노드가 totalNode에 없으면 false, 있으면 true.
	int num;	// 지역번호
	string name;	// 지역이름
	bool isFlood;	// 침수여부

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

// 문자열 파싱 함수
vector<string> parsing(string str) {
	int previous = 0;
	int current = 0;
	vector<string> x;
	x.clear();

	current = str.find(' ');
	//find는 찾을게 없으면 npos를 리턴함
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


Node totalNode[1000000];	// 전체 노드를 담는 배열
// graph[n] 에는 지역 번호 n번 노드에 연결된 노드들이 인접리스트 구조로 저장되어있다.
// pair<Node, int> -> Node : 인접 노드, int : 거리
vector< pair<Node, int> > graph[1000000];	// 노드 번호는 100,000 ~ 999,999 사이의 정수.
int d[1000000];	// 최단거리 테이블

// N과 trace를 동시에 return 해주기위해 만든 구조체.
struct myStruct {
	int N;	// tree vertice 수
	vector<int>* trace;	// 최단거리가 업데이트 될때 바로 이전에 방문하게 되는 정점
};

class Navi {
private:	
	// n: 지역정보 수, m: 도로정보 수, q: 질의 수
	int n = 0;
	int m = 0;
	int q = 0;	
	int num = 0;


public:
	
	void insertNode(int _num, string _name, bool _isFlood) {	// 노드 입력
		Node temp = Node(_num, _name, _isFlood);
		totalNode[_num] = temp;
	}

	void insertEdge(int _num1, int _num2, int _distance) {	// 간선 입력
		graph[_num1].push_back(pair<Node, int>(totalNode[_num2], _distance));
		graph[_num2].push_back(pair<Node, int>(totalNode[_num1], _distance));
	}

	// 우선순위 큐를 이용한 다익스트라
	myStruct Dijkstra(int start) {	// 다익스트라 알고리즘
		myStruct result;
		vector<int>* from = new vector<int>(1000000);

		int N=0;	// tree vertice 수

		priority_queue< pair<int, int> > pq;	// 거리, 노드번호

		pq.push({ 0, start });
		N++;
		d[start] = 0;

		while (!pq.empty()) {
			int dist = -pq.top().first;	// 현재 노드까지의 비용
			int now = pq.top().second;	// 현재 노드
			// cout << "dist, now : " << dist << now << endl;
			pq.pop();

			if (d[now] < dist)	// 이미 최단경로 처리한 노드인 경우 패스
				continue;

			for (int i = 0; i < graph[now].size(); i++) {

				if (graph[now][i].first.isFlood) {	// 침수되었을 경우 제외.
					continue;
				}

				// cout << graph[now][i].first.num << "의 원래 d 값 : " << d[graph[now][i].first.num] << endl;
				// 이 노드를 거쳐가는 비용 계산
				int cost = dist + graph[now][i].second;
				// 비용이 더 작다면 최단경로 테이블 업데이트
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

		// 재귀호출을 통해 정점 e전의 정점에 대한 경로를 출력한다..  
		trace_path(s, (*from)[e], from);

		// 최단경로에서 정점 e 바로 이전의 정점를 화면에 출력한다.  
		cout << totalNode[(*from)[e]].name << " - > ";
	}
	
	// 마지막 목적지를 간편하게 화면의 출력하기 위해 함수 분리  
	void print_path(int s, int e, vector<int>* from) {
		// 위의 trace_path를 호출하여 최단 경로를 출력한후,  
		trace_path(s, e, from);

		// 목적지의 정점 번호도 출력한다.  
		cout << totalNode[e].name;
	}

	void shortestDistance(int S, int D) {	// 최단거리 찾기
		myStruct s = Dijkstra(S);
		if(d[D] >= INF){
			cout << "None" << endl;
		}
		else {
			cout << "최단거리 : " << d[D] << " " << totalNode[S].name << " " << totalNode[D].name << endl;
		}
	}

	void shortestPath(int S, int D) {	// 최단경로 찾기
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
		cout << "지역 정보 수, 도로 정보 수, 질의의 수를 입력하세요." << endl;
		cin >> n >> m >> q;

		this->n = n;
		this->m = m;
		this->q = q;

		for (int i = 0; i < n; i++) {	// 지역정보 입력받기
			cout << i + 1 << " 번째 지역정보 : " << endl;
			int a, c;
			string b;
			cin >> a >> b >> c;
			insertNode(a, b, c);
		}

		for (int i = 0; i < m; i++) {	// 도로정보 입력받기
			cout << i + 1 << " 번째 도로정보 :" << endl;
			int a, b, c;
			cin >> a >> b >> c;
			insertEdge(a, b, c);
		}



		/*cout << "체크할 번호 num:" << endl;
		int num;
		cin >> num;
		Dijkstra(num);
		cout << "다익스트라 실행. graph[num]에서 " << endl;
		for (int i = 0; i < graph[num].size(); i++) {
			cout << graph[num][i].first.num << " 까지의 거리는 "
				<< d[graph[num][i].first.num] << endl;
		}
		cout << endl;*/

		cin.ignore();

		for (int i = 0; i < q; i++) {	// 질의 입력받기
			// 최단거리 테이블 초기화
			fill(d, d + 1000000, INF);

			cout << i + 1 << " 번째 질의 : " << endl;
			string cmd;
			getline(cin, cmd);
			vector<string> cmdVector = parsing(cmd);
			
			if (cmdVector[0] == "A") {	// 최단거리 찾기
				//cout << "A를 입력했습니다" << endl;
				//cout << cmdVector[1] << " 부터 " << cmdVector[2] << "까지의 최단거리 : ";
				shortestDistance(stoi(cmdVector[1]), stoi(cmdVector[2]));

			}
			else if (cmdVector[0] == "B") {	// 최단경로 찾기
				//cout << "B를 입력했습니다" << endl;
				cout << " == 최단 경로를 탐색합니다 == " << endl;
				cout << "경로 : ";
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

