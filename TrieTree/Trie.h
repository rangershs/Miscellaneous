#ifndef TRIE_H
#define TRIE_H

#include <vector>
#include <list>
#include <string>

/*
	ʵ���ֵ����Ļ�������, ֧��26��Ӣ��Сд��ĸ, ���б�Ҫ, ����������չ�ַ���
	��ķ��������̰߳�ȫ��, ��֧�ֶ��߳�ͬʱ����ͬһ������
	��ʵ�е��ʳ��Ȳ�����, ���õݹ鷽ʽʵ��, ע��ݹ�ı߽�����
*/

class TrieTree
{
public:
	struct Node
	{
		char					ch;
		bool					isWord;
		std::list<Node*>		nextNodes;	/* �漰��������ɾ����, list����vector; �ʼѡ�����vector, ɾ������ʱָ������������½����ɾ, ����ɾfive, ��ôfileҲ�ᱻɾ��, ����ɾfile��û���� */
	};

public:
	TrieTree();
	~TrieTree();
	TrieTree(const TrieTree& oth) = delete;
	TrieTree& operator=(const TrieTree& oth) = delete;

public:
	bool	insertStr(const char* data);
	bool	searchStr(const char* data);
	bool	deleteStr(const char* data);
	void	traverseTree();

private:
	bool	strIsExist(const char* data);
	void	getNodeValue(Node* node, const std::string& data);
	void	deleteNode(Node* node, const char* data);

private:
	Node*		root;
};

#endif