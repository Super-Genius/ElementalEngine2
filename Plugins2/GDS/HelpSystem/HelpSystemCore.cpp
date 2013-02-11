///============================================================================
/// \file		HelpSystemCore.cpp
/// \brief		Implementation of Help System core.
/// \date		11-20-2007
/// \author		Dmitriy S. Sergeyev
/// \note   Elemental Engine
///         Copyright (c)  2005-2008 Signature Devices, Inc.
///
///         This code is redistributable under the terms of the EE License.
///
///         This code is distributed without warranty or implied warranty of
///			merchantability or fitness for a particular purpose. See the 
///			EE License for more details.
///
///         You should have received a copy of the EE License along with this
///			code; If not, write to Signature Devices, Inc.,
///			3200 Bridge Parkway Suite 102, Redwood City, CA 94086 USA.
///============================================================================

#include "stdafx.h"

bool CCaseInsensitiveLess::operator() (const std::string *left, const std::string *right) const
{
	std::string::const_iterator li = left->begin();
	std::string::const_iterator ri = right->begin();
	// static_cast specified due to known flaw in MSVS CRT, which causes
	// tolower() to fail with debug assertion, if character is >127.
	while ((li != left->end()) && (ri != right->end()) &&
		(tolower(static_cast<unsigned char>(*li)) == tolower(static_cast<unsigned char>(*ri))))
	{
		// This is better than pseudo-tricky *li++ and *ri++
		++li;
		++ri;
	}
	if ((li == left->end()) || (ri == right->end()))
	{
		return ri != right->end();
	}
	return tolower(static_cast<unsigned char>(*li)) < tolower(static_cast<unsigned char>(*ri));
}

CHelpSystemCore::CChm::CChm(const CString & fileName, std::auto_ptr<chm::chmfile> chmfile)
:	m_fileName(fileName), m_chmfile(chmfile.release())
{
}

void CHelpSystemCore::CChm::Destroy()
{
	delete m_chmfile;
	m_chmfile = NULL;
}

const CHelpSystemCore::CDocument * CHelpSystemCore::CChm::FindDocument(const std::string &path) const
{
	DOCUMENTS::const_iterator iter = m_documents.find(&path);
	if (iter == m_documents.end())
	{
		return NULL;
	}
	return &iter->second;
}

CHelpSystemCore::CDocument * CHelpSystemCore::CChm::FindDocument(const std::string &path)
{
	DOCUMENTS::iterator iter = m_documents.find(&path);
	if (iter == m_documents.end())
	{
		 return NULL;
	}
	return &iter->second;
}

CHelpSystemCore::CDocument * CHelpSystemCore::CChm::GetDocument(const std::string *path)
{
	ASSERT(path);
	return &m_documents.insert(std::make_pair(path, CDocument(this, path))).first->second;
}

CHelpSystemCore::CDocument * CHelpSystemCore::CChm::GetDocument(const std::string &path)
{
	DOCUMENTS::iterator iter = m_documents.find(&path);
	if (iter == m_documents.end())
	{
		m_pathCache.push_back(path);
		return &m_documents.insert(std::make_pair(&m_pathCache.back(), CDocument(this, &m_pathCache.back()))).first->second;
	}
	else
	{
		return &iter->second;
	}
}

CHelpSystemCore::CDocument::CDocument(CChm *pchm, const std::string *path)
:	m_chm(pchm), m_path(path), m_topicsTree(NULL), m_indexNode(NULL)
{
	ASSERT(m_chm);
	ASSERT(m_path);
}

CHelpSystemCore::CTopicsTree::CTopicsTree(CTopicsTree *parent, const std::string *title, CDocument *document)
:	m_parent(parent), m_title(title), m_document(document)
{
	ASSERT(m_title);
}

CHelpSystemCore::CIndexTopic::CIndexTopic(const std::string *title, CDocument *document)
:	m_title(title), m_document(document)
{
	ASSERT(m_title);
	ASSERT(m_document);
}

CHelpSystemCore::CIndexTree::CIndexTree(CIndexTree *parent, const std::string *name)
:	m_parent(parent), m_name(name)
{
	ASSERT(m_name);
}

std::string CHelpSystemCore::CIndexTree::GetFullName() const
{
	std::string fullName = *m_name;
	CIndexTree * current = m_parent;
	while (current)
	{
		fullName = *current->m_name + ", " + fullName;
		current = current->m_parent;
	}
	return fullName;
}

CHelpSystemCore::CHelpSystemCore(const CString & path)
{
	SearchCHMs(path);
}

CHelpSystemCore::~CHelpSystemCore()
{
	for (CHMLIST::iterator iter = m_chmList.begin(); iter != m_chmList.end(); ++iter)
	{
		iter->Destroy();
	}
}

CHelpSystemCore::CTopicsTree::CHILDRENITERATOR CHelpSystemCore::GetTreeBegin()
{
	if (!m_topicsTree.get())
	{
		BuildTopicsTree();
	}
	// Anyway topic tree should be created, even if empty.
	ASSERT(m_topicsTree.get());
	return m_topicsTree->begin();
}

CHelpSystemCore::CTopicsTree::CHILDRENITERATOR CHelpSystemCore::GetTreeEnd()
{
	if (!m_topicsTree.get())
	{
		BuildTopicsTree();
	}
	// Anyway topic tree should be created, even if empty.
	ASSERT(m_topicsTree.get());
	return m_topicsTree->end();
}

CHelpSystemCore::INDEXITERATOR CHelpSystemCore::GetIndexBegin()
{
	if (!m_index.get())
	{
		BuildTopicIndex();
	}
	// Anyway topic index should be created, even if empty.
	ASSERT(m_index.get());
	return m_index->begin();
}

CHelpSystemCore::INDEXITERATOR CHelpSystemCore::GetIndexEnd()
{
	if (!m_index.get())
	{
		BuildTopicIndex();
	}
	// Anyway topic index should be created, even if empty.
	ASSERT(m_index.get());
	return m_index->end();
}

const CHelpSystemCore::CChm * CHelpSystemCore::FindChmByName(const CString & name) const
{
	CString findName(name);
	findName.MakeLower();
	CHMMAP::const_iterator i = m_chmNames.find(findName);
	if (i == m_chmNames.end())
	{
		return NULL;
	}
	else
	{
		return &(*i->second);
	}
}

const CHelpSystemCore::CChm * CHelpSystemCore::FindChmByFullFilename(const CString & filename) const
{
	CString lowFilename(filename);
	lowFilename.MakeLower();
	for (CHMLIST::const_iterator i = m_chmList.begin(); i != m_chmList.end(); ++i)
	{
		CString chmFilename(i->GetFileName());
		chmFilename.MakeLower();
		if (lowFilename == chmFilename)
		{
			return &(*i);
		}
	}
	return NULL;
}

static size_t EqualCount(const std::string &left, const std::string &right)
{
	std::string::const_iterator li = left.begin();
	std::string::const_iterator ri = right.begin();
	size_t ec = 0;
	while ((li != left.end()) && (ri != right.end()) &&
		(tolower(static_cast<unsigned char>(*li)) == tolower(static_cast<unsigned char>(*ri))))
	{
		++li;
		++ri;
		++ec;
	}
	return ec;
}

CHelpSystemCore::INDEXITERATOR CHelpSystemCore::SearchBestIndex(
	CIndexTree::CHILDREN const & index, const std::string &searchString
)
{
	if (index.empty())
	{
		return index.end();
	}
	// boost::multi_index or search-tree is more suitable for this task.
	// But for now the following stub is used, which performs quite fast
	// searching with some help of map.
	std::string str(searchString);
	size_t ec = 0;
	INDEXITERATOR best;
	while (!str.empty())
	{
		INDEXITERATOR iter = index.lower_bound(&str);
		if (iter != index.end())
		{
			size_t n = EqualCount(searchString, *iter->first);
			if (n < ec)
			{
				break;
			}
			ec = n;
		}
		best = iter;
		str.resize(str.size() - 1);
	}
	if (str.empty() && (ec == 0))
	{
		return index.begin();
	}
	return best;
}

const CHelpSystemCore::CIndexTree * CHelpSystemCore::SearchIndex(const std::string &searchString)
{
	if (!m_index.get())
	{
		BuildTopicIndex();
	}
	std::string searchFor(searchString);
	std::string tmp;
	// Walk to index node, which specified by ',' delimiters.
	CIndexTree::CHILDREN * nodes = m_index.get();
	for (;;)
	{
		size_t delim = searchFor.find(',');
		if (delim == std::string::npos)
		{
			break;
		}
		int end = delim - 1;
		while ((end > 0) && (isspace(static_cast<unsigned char>(searchFor[end])))) --end;
		tmp = searchFor.substr(0, end + 1);
		CIndexTree::CHILDREN::iterator iter = nodes->find(&tmp);
		if (iter == nodes->end())
		{
			break;
		}
		if (iter->second.GetChildren().empty())
		{
			return &iter->second;
		}
		nodes = &iter->second.GetChildren();
		end = delim + 1;
		while ((static_cast<size_t>(end) < searchFor.size()) &&
			(isspace(static_cast<unsigned char>(searchFor[end])))) ++end;
		searchFor.erase(0, end);
	}

	// Search for best conforomance with remanied part of search string.
	INDEXITERATOR iter = SearchBestIndex(*nodes, searchFor);
	if (iter == nodes->end())
	{
		return NULL;
	}
	return &iter->second;
}

void CHelpSystemCore::Search(SEARCHRESULT &result, const std::string &txt, bool wholeWords, bool titlesOnly)
{
	typedef std::list<chm::chm_search_match> CHMFOUND;
	typedef std::vector<chm::chm_search_document> CHMDOCUMENTS;
	// Build everything to make all structures valid.
	if (!m_topicsTree.get())
	{
		BuildTopicsTree();
	}
	if (!m_index.get())
	{
		BuildTopicIndex();
	}
	// Already found documents set.
	std::set<CDocument *> foundSet;
	// Perform searching.
	for (CHMLIST::iterator iter = m_chmList.begin(); iter != m_chmList.end(); ++iter)
	{
		CHMFOUND found;
		if (iter->GetChmFile().search_index(txt, found, wholeWords, titlesOnly))
		{
			for (CHMFOUND::iterator match = found.begin(); match != found.end(); ++match)
			{
				for (CHMDOCUMENTS::iterator chmdoc = match->documents.begin(); chmdoc != match->documents.end(); ++chmdoc)
				{
					CDocument * doc = iter->GetDocument(chmdoc->path);
					ASSERT(doc);
					if (foundSet.insert(doc).second)
					{
						result.push_back(CSearchMatch());
						CSearchMatch &sm = result.back();
						sm.SetDocument(doc);
						sm.SetKey(match->key);
						sm.SetTitle(chmdoc->title);
						sm.SetOffsets(chmdoc->offsets);
					}
				}
			}
		}
	}
}

bool CHelpSystemCore::SearchCHMs(const CString &path)
{
	// Retrieive full path with wildcard and full path without wildcard.
	CString fullPathWildcard;
	CString fullPath;
	// Construct full path with wildcard.
	int pos = DLLModuleName.ReverseFind('\\');
	if (pos < 0)
	{
		// Invalid path.
		return false;
	}
	fullPathWildcard = DLLModuleName.Left(pos + 1);
	fullPathWildcard += path;
	// Retrieveing full path without wildcard.
	pos = fullPathWildcard.ReverseFind('\\');
	if (pos < 0)
	{
		// Invalid path (shouldn't actually get here).
		return false;
	}
	fullPath = fullPathWildcard.Left(pos + 1);

	// Search and load CHM files.
	bool bIsFound = false;
	WIN32_FIND_DATA fdData;
	HANDLE hFind = FindFirstFile(fullPathWildcard, &fdData);
	if (hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			if ((fdData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0)
			{
				CString fileName = fullPath + fdData.cFileName;
				// Here (const char *) cast is used, because chm::chmfile
				// supports std::string only.
				std::auto_ptr<chm::chmfile> chmFile(
					new chm::chmfile((const char *)(fileName))
				);
				if (chmFile->is_open())
				{
					// The CHM is open and valid.
					CString name(fdData.cFileName);
					// Remove extension.
					int ppos = name.ReverseFind('.');
					if (ppos >= 0)
					{
						name.Delete(ppos, name.GetLength());
					}
					name.MakeLower();
					if (m_chmNames.find(name) == m_chmNames.end())
					{
						bIsFound = true;
						m_chmList.push_back(CChm(fileName, chmFile));
						CHMLIST::iterator item = m_chmList.end();
						--item;
						m_chmNames.insert(std::make_pair(name, item));
					}
					else
					{
						// TODO: Warning, CHM with such name already exists.
					}
				}
			}
		} while (FindNextFile(hFind, &fdData));
	}

	return bIsFound;
}

void CHelpSystemCore::BuildTopicsTree()
{
	if (m_topicsTree.get())
	{
		// TODO: Try to perform topic tree rebuild. This is an error.
		return;
	}
	typedef list<chm::chm_topics_tree *>::const_iterator nodeIter;
	m_topicsTree.reset(new CTopicsTree::CHILDREN);
	for (CHMLIST::iterator iter = m_chmList.begin(); iter != m_chmList.end(); ++iter)
	{
		if (iter->GetChmFile().get_topics_tree())
		{
			MergeTopicsTree(
				*iter,
				iter->GetChmFile().get_topics_tree()->children.begin(),
				iter->GetChmFile().get_topics_tree()->children.end(),
				NULL, *m_topicsTree
			);
		}
	}
}

void CHelpSystemCore::MergeTopicsTree(
	CChm &chmRef, chmTopicsTreeIter chmBegin, chmTopicsTreeIter chmEnd,
	CTopicsTree * parent, CTopicsTree::CHILDREN &topicsTree
)
{
	for (; chmBegin != chmEnd; ++chmBegin)
	{
		const std::string *nodeTitle = &(*chmBegin)->title;
		CTopicsTree::CHILDREN::iterator node;
		// Search for node with same name.
		for (node = topicsTree.begin(); node != topicsTree.end(); ++node)
		{
			if (node->GetTitle() == *nodeTitle)
			{
				break;
			}
		}
		if (node == topicsTree.end())
		{
			topicsTree.push_back(CTopicsTree(parent, nodeTitle, NULL));
			node = topicsTree.end();
			--node;
		}
		if (!(*chmBegin)->path.empty())
		{
			if (node->GetDocument())
			{
				// TODO: Error! Can't merge nodes with non-empty topic.
			}
			else
			{
				CDocument * doc = chmRef.GetDocument(&(*chmBegin)->path);
				doc->SetTopicsTree(&(*node));
				node->SetDocument(doc);
			}
		}
		if (!(*chmBegin)->children.empty())
		{
			MergeTopicsTree(
				chmRef, (*chmBegin)->children.begin(), (*chmBegin)->children.end(),
				&(*node), node->GetChildren()
			);
		}
	}
}

void CHelpSystemCore::BuildTopicIndex()
{
	if (m_index.get())
	{
		// TODO: Try to perform index rebuild. This is an error.
		return;
	}
	typedef list<chm::chm_index_tree *>::const_iterator nodeIter;
	m_index.reset(new CIndexTree::CHILDREN);
	for (CHMLIST::iterator iter = m_chmList.begin(); iter != m_chmList.end(); ++iter)
	{
		if (iter->GetChmFile().get_index())
		{
			MergeTopicIndex(
				*iter, iter->GetChmFile().get_index()->children.begin(),
				iter->GetChmFile().get_index()->children.end(), NULL, *m_index
			);
		}
	}
	// Sequental enumeration
	std::stack<std::pair<CIndexTree::CHILDREN::iterator, CIndexTree::CHILDREN::iterator> > index;
	index.push(std::make_pair(m_index->begin(), m_index->end()));
	int num = 0;
	while (!index.empty())
	{
		while (index.top().first != index.top().second)
		{
			index.top().first->second.SetSeqNumber(num);
			++num;
			index.push(std::make_pair(
				index.top().first->second.GetChildren().begin(),
				index.top().first->second.GetChildren().end()
			));
		}
		index.pop();
		if (!index.empty())
		{
			++index.top().first;
		}
	}
}

void CHelpSystemCore::MergeTopicIndex(
	CChm &chmRef, chmIndexIter chmBegin, chmIndexIter chmEnd,
	CIndexTree *parent, CIndexTree::CHILDREN &indexNodes
)
{
	for (; chmBegin != chmEnd; ++chmBegin)
	{
		const std::string *nodeName = &(*chmBegin)->name;
		CIndexTree::CHILDREN::iterator node = indexNodes.find(nodeName);
		if (node == indexNodes.end())
		{
			node = indexNodes.insert(std::make_pair(nodeName, CIndexTree(parent, nodeName))).first;
		}
		for (std::list<chm::chm_index_topic>::const_iterator i = (*chmBegin)->topics.begin(); i != (*chmBegin)->topics.end(); ++i)
		{
			CDocument * doc = chmRef.GetDocument(&i->path);
			doc->SetIndexNode(&node->second);
			node->second.GetTopics().push_back(CIndexTopic(&i->title, doc));
		}
		if (!(*chmBegin)->children.empty())
		{
			MergeTopicIndex(
				chmRef, (*chmBegin)->children.begin(), (*chmBegin)->children.end(),
				&node->second, node->second.GetChildren()
			);
		}
	}
}
