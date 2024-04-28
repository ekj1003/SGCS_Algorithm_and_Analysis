#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cstring>
#include <ctime>
#include <string>
#include <map>
#include <stack>
#include <queue>
using namespace std;

typedef struct node {
    char character;
    int freq;
    struct node *left;
    struct node *right;
} Node;

struct compare {
    bool operator()(Node *a, Node *b)
    {
        return a->freq > b->freq;
    }
};

void createNodeList(map<char, int> char_freq, priority_queue<Node *, vector<Node *>, compare>& char_info) {
    for (auto iter = char_freq.begin(); iter != char_freq.end(); iter++) {
        Node *nd = new Node;
        nd->character = iter->first;
        nd->freq = iter->second;
        nd->left = NULL;
        nd->right = NULL;
        char_info.push(nd);
    }
}

// Huffman tree 구성
Node* buildHuffmanTree(priority_queue<Node *, vector<Node *>, compare>& char_info) {
    while (char_info.size() != 1) {
        Node *nd1 = char_info.top();
        char_info.pop();
        Node *nd2 = char_info.top();
        char_info.pop();
        Node *nd3 = (Node *)malloc(sizeof(Node));
        nd3->character = '0';
        nd3->freq = nd1->freq + nd2->freq;
        nd3->left = nd1;
        nd3->right = nd2;
        char_info.push(nd3);
    }
    return char_info.top();
}

//Huffman 트리를 기반으로 각 문자에 해당하는 Huffman 코드를 생성
void generateHuffmanCode(Node* h_tree, map<char, string>& code_converted) {
    stack<pair<Node*, string> > st;
    st.push({h_tree, ""});

    while (!st.empty()) {
        Node* now = st.top().first;
        string s = st.top().second;
        st.pop();

        if (now->left != NULL)
            st.push({now->left, s + "0"});
        if (now->right != NULL)
            st.push({now->right, s + "1"});
        if (now->left == NULL && now->right == NULL)
            code_converted[now->character] = s;
    }
    
}

void decompress_data(FILE *fp, FILE *comp_file, int c_size, int c_cnt) {
    int count = 0;
    int i = 0;

    // 코드를 변환하기 위한 데이터 저장용 벡터
    vector<pair<pair<char, int>, string> > code_converted(c_cnt);

    // c_cnt만큼 반복하여 코드 변환 데이터 읽어오기
    while (i < c_cnt) {
        fscanf(fp, "%hhd %d\n", &code_converted[i].first.first, &code_converted[i].first.second);
        count += code_converted[i].first.second;
        i++;
    }

    // 문자열 데이터 읽어오기
    string s = "";
    i = 0;
    while (i < count) {
        char tmp = fgetc(fp);
        s += tmp;
        i++;
    }

    // 코드 변환된 문자열 저장
    int k = 0;
    i = 0;
    while (i < c_cnt) {
        int num = code_converted[i].first.second;
        string tmp = "";
        int j = k;

        while (j < k + num) {
            tmp += s[j];
            j++;
        }
        k = k + num;
        code_converted[i].second = tmp;
        i++;
    }

    // 비트 단위 데이터 읽어오기 및 압축 진행
    string h_code = "";
    int buf_size = c_size / 8;
    if (c_size % 8 != 0)
        buf_size++;

    for (int i = 0; i < buf_size; i++) {
        char c = fgetc(fp);
        unsigned char tmp = (unsigned char)c;
        unsigned char cmp = 128;
        for (int i = 0; i < 8; i++) {
            if ((int)(tmp & cmp) == 0)
                h_code += '0';
            else
                h_code += '1';
            tmp = tmp << 1;
        }
    }

    // 압축된 데이터 쓰기
    string part = "";
    for (int i = 0; i < c_size; i++) {
        part += h_code[i];
        for (int j = 0; j < c_cnt; j++) {
            if (part == code_converted[j].second) {
                fprintf(comp_file, "%c", code_converted[j].first.first);
                part = "";
            }
        }
    }
}



int main(int argc, char **argv){
    if(argc!=3){
		printf("input error!!");
		return 0;
	}

    char *cmd;
    char *in_name;
    cmd = argv[1];
    in_name = argv[2];
    char out_name[50] = "";

    FILE *fp = fopen(in_name, "r");
    
    strcat(out_name, in_name);

    if (!strcmp(cmd, "-c")) {
        map<char, int> char_freq;
        char ch;

        while ((ch = fgetc(fp)) != EOF) {
            char_freq[ch]++;
        }
        fclose(fp);

        priority_queue<Node *, vector<Node *>, compare> char_info;

        createNodeList(char_freq, char_info);
        Node* h_tree = buildHuffmanTree(char_info);
        
        map<char, string> code_converted;
        generateHuffmanCode(h_tree, code_converted);

        if (char_freq.size() == 1) code_converted[h_tree->character] = "0";

        fp = fopen(in_name, "r");
        string h_code;
        while ((ch = fgetc(fp)) != EOF){
            h_code += code_converted[ch];
        }

        strcat(out_name, ".zz");
        FILE *comp_file = fopen(out_name, "wb");
        if (comp_file != NULL) {
            // Write Huffman code size and converted code size to the file
            long int h_size = h_code.size();
            long int cc_size = code_converted.size();
            fprintf(comp_file, "%ld %ld\n", h_size, cc_size);

            // Write each character and its corresponding Huffman code length
            for (map<char, string>::iterator iter2 = code_converted.begin(); iter2 != code_converted.end(); iter2++) {
                int f = iter2->first;
                long int sl = iter2->second.length();
                fprintf(comp_file, "%d %ld\n", f, sl);
            }

            // Write the actual Huffman codes
            for (map<char, string>::iterator iter3 = code_converted.begin(); iter3 != code_converted.end(); iter3++) {
                string s = iter3->second;
                fprintf(comp_file, "%s", s.c_str()); // Writing the entire Huffman code string at once
            }

        }

        const int BITS_IN_BYTE = 8;
        unsigned char buf = 0; // 8비트의 이진 데이터를 저장할 변수
        int cnt = 0, i =0; // 이진 데이터의 비트 수를 카운트
        while(i<h_code.size()) {
            buf = buf << 1; // 이전 데이터를 왼쪽으로 이동하여 공간을 만듦
            buf = buf | (h_code[i] - '0'); // h_code의 해당 비트를 buf에 추가
            cnt++; // 비트 수 증가

            if (cnt == BITS_IN_BYTE) { // 8비트가 모두 채워지면
                fprintf(comp_file, "%c", buf); // buf의 이진 데이터를 파일에 씀
                buf = 0; // buf 초기화
                cnt = 0; // 카운트 초기화
            }
            i++;
        }

        // 남은 비트를 8비트로 맞춰서 파일에 씀
        buf = buf << (BITS_IN_BYTE - cnt); // 남은 비트를 왼쪽으로 이동하여 8비트 맞춤
        fprintf(comp_file, "%c", buf);


        fclose(comp_file);
        fclose(fp);
    }
    else if (!strcmp(cmd, "-d")) {
        int c_size, c_cnt;
        
        fscanf(fp, "%d %d\n", &c_size, &c_cnt);

        strcat(out_name, ".yy");
        FILE *comp_file = fopen(out_name, "wb");
        decompress_data(fp, comp_file, c_size, c_cnt);

        fclose(comp_file);
        fclose(fp);
    }
    else {
        cout << "input error!\n";
        fclose(fp);
        return 0;
    }
    return 0;
}