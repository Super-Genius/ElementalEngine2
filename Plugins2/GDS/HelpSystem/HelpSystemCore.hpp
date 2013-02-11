///============================================================================
/// \file		HelpSystemCore.hpp
/// \brief		Declaration of Help System core.
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

#ifndef _HELPSYSTEMCORE_HPP_
#define _HELPSYSTEMCORE_HPP_

template <class key>
class CIndirectedLess
{
public:
	bool operator() (const key &left, const key &right) const
	{ return *left < *right; }
};

class CCaseInsensitiveLess
{
public:
	bool operator() (const std::string *left, const std::string *right) const;
};

/// Help system core class.
/// Note: everywhere std::string* is specified, it means that member or
/// method argument of this type should be a point to string, which lifetime is
/// longer then object, such member or method belongs to. This done to
/// speedup various structures building and accessing.
class CHelpSystemCore
{
public:

	class CTopicsTree;
	class CIndexTree;

	class CChm;

	/// Help document properties.
	class CDocument
	{
	public:

		/// Get CHM file properties.
		const CChm &GetChm() const { return *m_chm; }

		/// Get path to document.
		const std::string &GetPath() const { return *m_path; }

		/// Get topic tree node related to this document. Returns NULL if
		/// this document not exists in topic tree.
		const CTopicsTree *GetTopicsTree() const { return m_topicsTree; }

		/// Set topic tree node related to this document.
		void SetTopicsTree(CTopicsTree *topicsTree) { m_topicsTree = topicsTree; }

		/// Get index node related to this document. Returns NULL if this
		/// document not listed in topic index.
		const CIndexTree *GetIndexNode() const { return m_indexNode; }

		/// Set index node related to this document.
		void SetIndexNode(CIndexTree *indexNode) { m_indexNode = indexNode; }

		/// Create document properties.
		/// \param[in] pchm CHM file properties -- must not be NULL.
		/// \param[in] path Path to document within CHM file -- must not be NULL.
		CDocument(CChm *pchm, const std::string *path);

	private:
		CChm *m_chm;
		const std::string *m_path;
		const CTopicsTree *m_topicsTree;
		const CIndexTree *m_indexNode;
	};

	/// Topics tree node.
	class CTopicsTree
	{
	public:
		typedef std::list<CTopicsTree> CHILDREN;
		typedef CHILDREN::const_iterator CHILDRENITERATOR;

		/// Get node title.
		const std::string & GetTitle() const { return *m_title; }

		/// Get document related to this topic tree element. NULL if no
		/// associated document.
		const CDocument * GetDocument() const { return m_document; }

		/// Set document related to this topic tree element.
		void SetDocument(CDocument * document) { m_document = document; }

		/// True if node has no children.
		bool IsLeaf() const { return m_children.empty(); }

		/// Get begin iterator for children nodes.
		CHILDRENITERATOR GetChildBegin() const { return m_children.begin(); }

		/// Get end iterator for children nodes.
		CHILDRENITERATOR GetChildEnd() const { return m_children.end(); }

		/// Get parent. NULL if it is root node.
		const CTopicsTree * GetParent() const { return m_parent; }

		/// Access to children.
		CHILDREN & GetChildren() { return m_children; }

		/// Create topic tree node.
		/// \param[in] parent Pointer to parent node.
		/// \param[in] title Title -- must nod be NULL.
		/// \param[in] document Pointer to document, or NULL if no document
		/// associated.
		CTopicsTree(CTopicsTree *parent, const std::string *title, CDocument *document);

	private:
		CTopicsTree *m_parent;
		const std::string *m_title;
		CDocument *m_document;
		CHILDREN m_children;
	};

	/// Index topic.
	class CIndexTopic
	{
	public:
		/// Get topic title.
		const std::string & GetTitle() const { return *m_title; }

		/// Get document related to this topic.
		const CDocument & GetDocument() const { return *m_document; }

		/// Create index topic element.
		CIndexTopic(const std::string *title, CDocument *document);

	private:
		const std::string *m_title;
		CDocument *m_document;
	};

	/// Index tree node.
	class CIndexTree
	{
	public:
		typedef std::list<CIndexTopic> TOPICLIST;
		typedef TOPICLIST::const_iterator TOPICITERATOR;

		typedef std::map<const std::string *, CIndexTree, CCaseInsensitiveLess > CHILDREN;
		typedef CHILDREN::const_iterator CHILDRENITERATOR;

		/// Get index node name.
		const std::string & GetName() const { return *m_name; }

		/// Get topics count.
		size_t GetTopicCount() const { return m_topics.size(); }

		/// True if some topics associated with this index node.
		bool IsTopic() const { return !m_topics.empty(); }

		/// Get index node transparent number, which corresponds to sequental
		/// enumeration of all tree elements.
		int GetSeqNumber() const { return m_seqNumber; }

		/// Set index node transparent number, which corresponsd to sequental
		/// enumeration of all tree elements.
		void SetSeqNumber(int seqNumber) { m_seqNumber = seqNumber; }

		/// Get full topic name including all parent topics.
		std::string GetFullName() const;

		/// Get topics begin.
		TOPICITERATOR GetTopicBegin() const { return m_topics.begin(); }

		/// Get topics end.
		TOPICITERATOR GetTopicEnd() const { return m_topics.end(); }

		/// Get children begin.
		CHILDRENITERATOR GetChildBegin() const { return m_children.begin(); }

		/// Get children end.
		CHILDRENITERATOR GetChildEnd() const { return m_children.end(); }

		/// Access to children.
		CHILDREN & GetChildren() { return m_children; }

		/// Access to list of topics.
		TOPICLIST & GetTopics() { return m_topics; }
		
		/// Create index tree node.
		CIndexTree(CIndexTree *parent, const std::string *name);
		
	private:
		CIndexTree *m_parent;
		const std::string *m_name;
		CHILDREN m_children;
		TOPICLIST m_topics;
		int m_seqNumber;
	};

	/// CHM file representation.
	class CChm
	{
	public:
		/// Documents path-properties association.
		typedef std::map<const std::string *, CDocument, CIndirectedLess<const std::string *> > DOCUMENTS;

		/// Get fully qualified path and filename of CHM.
		const CString &GetFileName() const { return m_fileName; }

		/// Get chmfile instance.
		const chm::chmfile &GetChmFile() const { return *m_chmfile; }

		/// Const find document by path.
		/// \return Pointer to found document, NULL otherwise.
		const CDocument * FindDocument(const std::string &path) const;
		
		/// Find document by path.
		/// \return Pointer to found document, NULL otherwise.
		CDocument * FindDocument(const std::string &path);

		/// Get document properties with specified path or create new
		/// document properties for specified path.
		/// \param[in] path Path -- must not be NULL.
		/// \return Pointer to document.
		CDocument * GetDocument(const std::string *path);

		/// Get document properties with specified path or create new
		/// document properties for specified path.
		/// Unlike GetDocument(const std::string *) it allows to specify
		/// any string, which instance is not exists anywhere. Document
		/// created in this case will contain pointer to string inside
		/// path cache.
		/// \param[in] path Path to document.
		/// \return Pointer to document.
		CDocument * GetDocument(const std::string &path);

		/// Destroy CHM file.
		void Destroy();

		/// Create CHM file properties.
		/// \param[in] fileName Fully qualified path and filename of CHM.
		/// \param[in] chmfile Successfully opened and valid CHM.
		CChm(const CString &fileName, std::auto_ptr<chm::chmfile> chmfile);

	private:
		/// Fully qualified path and filename of CHM.
		CString m_fileName;
		/// Successfully opened and valid CHM.
		chm::chmfile *m_chmfile;
		/// Documents container.
		DOCUMENTS m_documents;
		/// Documents path cache.
		std::list<std::string> m_pathCache;
	};

	/// Class represents search math item.
	class CSearchMatch
	{
	public:
		/// Offsets where results found.
		typedef std::vector<int> OFFSETS;

		/// Get document reference.
		const CDocument &GetDocument() const { return *m_document; }

		/// Get key that was found.
		const std::string &GetKey() const { return m_key; }

		/// Get title of document.
		const std::string &GetTitle() const { return m_title; }

		/// Get offsets where keyword found.
		const OFFSETS &GetOffsets() const { return m_offsets; }

		/// Set document.
		void SetDocument(CDocument *document) { m_document = document; }

		/// Set key.
		void SetKey(const std::string &key) { m_key = key; }

		/// Set title.
		void SetTitle(const std::string &title) { m_title = title; }

		/// Set offsets.
		void SetOffsets(const OFFSETS &offsets) { m_offsets = offsets; }

	private:
		/// Document pointer.
		CDocument *m_document;
		/// Key that was found.
		std::string m_key;
		/// Document title.
		std::string m_title;
		/// Offsets where results found.
		OFFSETS m_offsets;
	};

	typedef CTopicsTree::CHILDRENITERATOR TOPICITERATOR;
	typedef CIndexTree::CHILDRENITERATOR INDEXITERATOR;

	typedef std::list<CChm> CHMLIST;
	typedef CHMLIST::const_iterator CHMITERATOR;

	typedef std::list<CSearchMatch> SEARCHRESULT;

	/// Get begin iterator for CHM files.
	CHMITERATOR GetChmBegin() const { return m_chmList.begin(); }
	/// Get end iterator for CHM files.
	CHMITERATOR GetChmEnd() const { return m_chmList.end(); }

	/// Get CHM pointer by name, (equals to filename without ".chm" extension).
	/// Returns NULL if no CHM with such name found.
	const CChm * FindChmByName(const CString &name) const;

	/// Get CHM pointer by full CHM filename.
	/// Returns NULL if no such CHM found.
	const CChm * FindChmByFullFilename(const CString &name) const;

	/// Search for most appropriate index node which corresponds to specified
	/// string.
	const CIndexTree * SearchIndex(const std::string &searchString);

	/// Perform search through all CHMs.
	void Search(SEARCHRESULT &result, const std::string &txt, bool wholeWords, bool titlesOnly);

	/// Get begin iterator for topics tree root nodes.
	TOPICITERATOR GetTreeBegin();
	/// Get end iterator for topics tree root nodes.
	TOPICITERATOR GetTreeEnd();

	/// Get begin iterator for index list.
	INDEXITERATOR GetIndexBegin();
	/// Get end iterator for index list.
	INDEXITERATOR GetIndexEnd();

	/// Initialize help system.
	/// \param[in] path Relative path to search for .CHM files and load index of
	/// them. Wildcard MUST be specified.
	CHelpSystemCore(const CString &path);

	/// Destroy help system.
	~CHelpSystemCore();

private:
	typedef list<chm::chm_topics_tree *>::const_iterator chmTopicsTreeIter;
	typedef list<chm::chm_index_tree *>::const_iterator chmIndexIter;

	typedef map<CString, CHMLIST::iterator> CHMMAP;

	bool SearchCHMs(const CString &path);
	void BuildTopicsTree();
	void MergeTopicsTree(
		CChm &chmRef, chmTopicsTreeIter chmBegin, chmTopicsTreeIter chmEnd,
		CTopicsTree *parent, CTopicsTree::CHILDREN &topicsTree
	);

	INDEXITERATOR SearchBestIndex(CIndexTree::CHILDREN const & index, const std::string &searchString);

	void BuildTopicIndex();
	void MergeTopicIndex(
		CChm &chmRef, chmIndexIter chmBegin, chmIndexIter chmEnd,
		CIndexTree *parent, CIndexTree::CHILDREN &indexNodes
	);

	CHMLIST m_chmList;
	std::auto_ptr<CTopicsTree::CHILDREN> m_topicsTree;
	std::auto_ptr<CIndexTree::CHILDREN> m_index;
	CHMMAP m_chmNames;
};

#endif // #ifndef _HELPSYSTEMCORE_HPP_
