/*
 * This source file is part of an OSTIS project. For the latest info, see http://ostis.net
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

/*!
 * @file sc_memory.hpp
 *
 * @brief This file contains the C++ API for managing the sc-memory.
 */

#pragma once

extern "C"
{
#include "sc-core/sc_memory_headers.h"
#include "sc-core/sc_helper.h"
}

#include "sc_addr.hpp"
#include "sc_event.hpp"
#include "sc_iterator.hpp"
#include "sc_stream.hpp"
#include "sc_template.hpp"
#include "sc_type.hpp"

class ScMemoryContext;

typedef struct
{
  ScAddr addr1;
  ScAddr addr2;
  ScAddr addr3;
  ScAddr addr4;
  ScAddr addr5;
} ScSystemIdentifierQuintuple;

SC_DEPRECATED(0.9.0, "Use ScSystemIdentifierQuintuple instead of. It will be removed in 0.10.0")
typedef ScSystemIdentifierQuintuple ScSystemIdentifierFiver;

class ScMemory
{
  friend class ScMemoryContext;

public:
  /*!
   * @brief Initializes the sc-memory system with the specified parameters.
   *
   * This function initializes the sc-memory system with the provided parameters.
   * It sets up the global sc-memory context, initializes keynodes, and sets up logging.
   *
   * @param params The parameters for initializing the sc-memory system.
   * @return Returns true if the initialization is successful; otherwise, returns false.
   */
  _SC_EXTERN static bool Initialize(sc_memory_params const & params);

  /*!
   * @brief Checks if the sc-memory system is initialized.
   *
   * This function checks whether the sc-memory system is initialized.
   * It returns true if the system is initialized; otherwise, it returns false.
   *
   * @return Returns true if the sc-memory system is initialized; otherwise, returns false.
   */
  _SC_EXTERN static bool IsInitialized();

  /*!
   * @brief Shuts down the sc-memory system.
   *
   * This function shuts down the sc-memory system, performing necessary cleanup operations.
   * It optionally saves the state of the system before shutting down.
   *
   * @param saveState If true, the current state of the sc-memory system is saved before shutting down.
   * @return Returns true if the shutdown is successful; otherwise, returns false.
   */
  _SC_EXTERN static bool Shutdown(bool saveState = true);

  _SC_EXTERN static void LogMute();
  _SC_EXTERN static void LogUnmute();

protected:
  static ScMemoryContext * ms_globalContext;
};

//! Class used to work with memory. It provides functions to create/retrieve/erase sc-elements
class ScMemoryContext
{
public:
  struct ScMemoryStatistics
  {
    sc_uint64 m_nodesNum;
    sc_uint64 m_linksNum;
    sc_uint64 m_edgesNum;

    sc_uint64 GetAllNum() const
    {
      return m_nodesNum + m_linksNum + m_edgesNum;
    }
  };

public:
  SC_DEPRECATED(
      0.10.0,
      "Don't use this method for creating sc-memory context. Use one with `userAddr` params instead of. "
      "This function will be deleted in 0.11.0")
  _SC_EXTERN explicit ScMemoryContext(sc_uint8 accessLevels, std::string const & name = "");
  SC_DEPRECATED(
      0.10.0,
      "Don't use this method for creating sc-memory context. Use one with `userAddr` params instead of. "
      "This function will be deleted in 0.11.0")
  _SC_EXTERN explicit ScMemoryContext(std::string const & name);
  _SC_EXTERN explicit ScMemoryContext(ScAddr const & userAddr = ScAddr::Empty);
  _SC_EXTERN explicit ScMemoryContext(sc_memory_context * context);
  _SC_EXTERN ~ScMemoryContext();

  // Disable object copying
  ScMemoryContext(ScMemoryContext const & other) = delete;
  ScMemoryContext & operator=(ScMemoryContext const & other) = delete;

  _SC_EXTERN sc_memory_context * operator*() const
  {
    return m_context;
  }

  _SC_EXTERN sc_memory_context * GetRealContext() const
  {
    return m_context;
  }

  //! Call this function, when you request to destroy real memory context, before destructor calls for this object
  _SC_EXTERN void Destroy();

  //! Begin events pending mode
  _SC_EXTERN void BeingEventsPending();

  //! End events pending mode
  _SC_EXTERN void EndEventsPending();

  /*!
   * @brief Checks if the sc-memory context is valid.
   *
   * This method checks whether the sc-memory context is valid.
   *
   * @return Returns true if the sc-memory context is valid; otherwise, returns false.
   *
   * @code
   * ScMemoryContext ctx;
   * if (ctx.IsValid()) {
   *     // Perform operations using the valid context
   * } else {
   *     // Handle invalid context
   * }
   * @endcode
   */
  _SC_EXTERN bool IsValid() const;

  /*!
   * @brief Checks if an sc-element exists with the specified address.
   *
   * This method checks whether an sc-element with the specified sc-address exists in the sc-memory.
   *
   * @param addr The sc-address to check for existence.
   * @return Returns true if the sc-element exists; otherwise, returns false.
   *
   * @code
   * ScMemoryContext ctx;
   * ScAddr elementAddr = ctx.CreateNode(ScType::NodeConst);
   * if (ctx.IsElement(elementAddr)) {
   *     // Element exists
   * } else {
   *     // Element does not exist
   * }
   * @endcode
   */
  _SC_EXTERN bool IsElement(ScAddr const & addr) const;

  /*!
   * @brief Returns the count of output arcs for a specified sc-element.
   *
   * This method retrieves the count of output arcs for the sc-element identified by the given sc-address.
   *
   * @param addr The sc-address of the sc-element to query.
   * @return Returns the count of output arcs for the specified sc-element.
   * @throws ExceptionInvalidParams if the specified sc-address is invalid.
   *
   * @code
   * ScMemoryContext ctx;
   * ScAddr elementAddr = ctx.CreateNode(ScType::NodeConst);
   * size_t outputArcsCount = ctx.GetElementOutputArcsCount(elementAddr);
   * std::cout << "Output Arcs Count: " << outputArcsCount << std::endl;
   * @endcode
   */
  _SC_EXTERN size_t GetElementOutputArcsCount(ScAddr const & addr) const noexcept(false);

  /*!
   * @brief Returns the count of input arcs for a specified sc-element.
   *
   * This method retrieves the count of input arcs for the sc-element identified by the given sc-address.
   *
   * @param addr The sc-address of the sc-element to query.
   * @return Returns the count of input arcs for the specified sc-element.
   * @throws ExceptionInvalidParams if the specified sc-address is invalid.
   *
   * @code
   * ScMemoryContext ctx;
   * ScAddr elementAddr = ctx.CreateNode(ScType::NodeConst);
   * size_t inputArcsCount = ctx.GetElementInputArcsCount(elementAddr);
   * std::cout << "Input Arcs Count: " << inputArcsCount << std::endl;
   * @endcode
   */
  _SC_EXTERN size_t GetElementInputArcsCount(ScAddr const & addr) const noexcept(false);

  /*!
   * @brief Erases an sc-element from the sc-memory.
   *
   * This method erases the sc-element identified by the given sc-address from the sc-memory.
   *
   * @param addr The sc-address of the sc-element to erase.
   * @return Returns true if the sc-element was successfully erased; otherwise, returns false.
   *
   * @code
   * ScMemoryContext ctx;
   * ScAddr elementAddr = ctx.CreateNode(ScType::NodeConst);
   * if (ctx.EraseElement(elementAddr)) {
   *     // Element successfully erased
   * } else {
   *     // Erase operation failed
   * }
   * @endcode
   */
  _SC_EXTERN bool EraseElement(ScAddr const & addr) noexcept(false);

  /*!
   * @brief Creates a new sc-node with the specified type.
   *
   * This method creates a new sc-node with the specified type and returns its sc-address.
   *
   * @param type The type of the sc-node to create.
   * @return Returns the sc-address of the newly created sc-node.
   * @throws ExceptionInvalidParams if the specified type is not a valid sc-node type or if sc-memory is full.
   *
   * @code
   * ScMemoryContext ctx;
   * ScAddr nodeAddr = ctx.CreateNode(ScType::NodeConst);
   * @endcode
   */
  _SC_EXTERN ScAddr CreateNode(ScType const & type) noexcept(false);

  /*!
   * @brief Creates a new sc-link with the specified type.
   *
   * This method creates a new sc-link with the specified type and returns its sc-address.
   *
   * @param type The type of the sc-link to create (default is ScType::LinkConst).
   * @return Returns the sc-address of the newly created sc-link.
   * @throws ExceptionInvalidParams if the specified type is not a valid sc-link type or if sc-memory is full.
   *
   * @code
   * ScMemoryContext ctx;
   * ScAddr linkAddr = ctx.CreateLink();
   * @endcode
   */
  _SC_EXTERN ScAddr CreateLink(ScType const & type = ScType::LinkConst) noexcept(false);

  /*!
   * @brief Creates a new sc-connector with the specified type, source, and target.
   *
   * This method creates a new sc-connector with the specified type, source, and target, and returns its sc-address.
   *
   * @param type The type of the sc-connector to create.
   * @param addrBeg The sc-address of the source sc-element.
   * @param addrEnd The sc-address of the target sc-element.
   * @return Returns the sc-address of the newly created sc-connector.
   * @throws ExceptionInvalidParams if the specified source or target sc-address is invalid or if sc-memory is full.
   *
   * @code
   * ScMemoryContext ctx;
   * ScAddr sourceNode = ctx.CreateNode(ScType::NodeConst);
   * ScAddr targetNode = ctx.CreateNode(ScType::NodeConst);
   * ScAddr edgeAddr = ctx.CreateEdge(ScType::EdgeDCommonConst, sourceNode, targetNode);
   * @endcode
   */
  _SC_EXTERN ScAddr CreateEdge(ScType const & type, ScAddr const & addrBeg, ScAddr const & addrEnd) noexcept(false);

  /*!
   * @brief Returns the type of the specified sc-element.
   *
   * This method retrieves the type of the sc-element identified by the given sc-address.
   *
   * @param addr The sc-address of the sc-element to query.
   * @return Returns the type of the specified sc-element.
   * @throws ExceptionInvalidParams if the specified sc-address is invalid.
   *
   * @code
   * ScMemoryContext ctx;
   * ScAddr elementAddr = ctx.CreateNode(ScType::NodeConst);
   * ScType elementType = ctx.GetElementType(elementAddr);
   * std::cout << "Element Type: " << elementType.GetName() << std::endl;
   * @endcode
   */
  _SC_EXTERN ScType GetElementType(ScAddr const & addr) const noexcept(false);

  /*!
   * @brief Changes the subtype of an sc-element.
   *
   * This method changes the subtype of the sc-element identified by the given sc-address.
   *
   * @param addr The sc-address of the sc-element to modify.
   * @param subtype The new subtype to set.
   * @return Returns true if the subtype was successfully changed; otherwise, returns false.
   * @throws ExceptionInvalidParams if the specified sc-address is invalid.
   *
   * @code
   * ScMemoryContext ctx;
   * ScAddr elementAddr = ctx.CreateNode(ScType::NodeConst);
   * if (ctx.SetElementSubtype(elementAddr, ScType::NodeConstRole)) {
   *     // Subtype successfully changed
   * } else {
   *     // Subtype change failed
   * }
   * @endcode
   */
  _SC_EXTERN bool SetElementSubtype(ScAddr const & addr, sc_type subtype) noexcept(false);

  /*!
   * @brief Returns the source sc-element of an sc-connector.
   *
   * This method retrieves the source sc-element of the sc-connector identified by the given sc-address.
   *
   * @param edgeAddr The sc-address of the sc-connector.
   * @return Returns the sc-address of the source sc-element.
   * @throws ExceptionInvalidParams if the specified sc-connector address is invalid.
   *
   * @code
   * ScMemoryContext ctx;
   * ScAddr sourceNode = ctx.CreateNode(ScType::NodeConst);
   * ScAddr targetNode = ctx.CreateNode(ScType::NodeConst);
   * ScAddr edgeAddr = ctx.CreateEdge(ScType::EdgeDCommonConst, sourceNode, targetNode);
   * ScAddr sourceElement = ctx.GetEdgeSource(edgeAddr);
   * @endcode
   */
  _SC_EXTERN ScAddr GetEdgeSource(ScAddr const & edgeAddr) const noexcept(false);

  /*!
   * @brief Returns the target sc-element of an sc-connector.
   *
   * This method retrieves the target sc-element of the sc-connector identified by the given sc-address.
   *
   * @param edgeAddr The sc-address of the sc-connector.
   * @return Returns the sc-address of the target sc-element.
   * @throws ExceptionInvalidParams if the specified sc-connector address is invalid.
   *
   * @code
   * ScMemoryContext ctx;
   * ScAddr sourceNode = ctx.CreateNode(ScType::NodeConst);
   * ScAddr targetNode = ctx.CreateNode(ScType::NodeConst);
   * ScAddr edgeAddr = ctx.CreateEdge(ScType::EdgeDCommonConst, sourceNode, targetNode);
   * ScAddr targetElement = ctx.GetEdgeTarget(edgeAddr);
   * @endcode
   */
  _SC_EXTERN ScAddr GetEdgeTarget(ScAddr const & edgeAddr) const noexcept(false);

  /*!
   * @brief Returns the source and target sc-elements of an sc-connector.
   *
   * This method retrieves the source and target sc-elements of the sc-connector identified by the given sc-address.
   *
   * @param edgeAddr The sc-address of the sc-connector.
   * @param outSourceAddr Reference to store the sc-address of the source sc-element.
   * @param outTargetAddr Reference to store the sc-address of the target sc-element.
   * @return Returns true if the information was successfully retrieved; otherwise, returns false.
   * @throws ExceptionInvalidParams if the specified sc-connector address is invalid.
   *
   * @code
   * ScMemoryContext ctx;
   * ScAddr sourceNode = ctx.CreateNode(ScType::NodeConst);
   * ScAddr targetNode = ctx.CreateNode(ScType::NodeConst);
   * ScAddr edgeAddr = ctx.CreateEdge(ScType::EdgeDCommonConst, sourceNode, targetNode);
   * ScAddr sourceElement, targetElement;
   * if (ctx.GetEdgeInfo(edgeAddr, sourceElement, targetElement)) {
   *     // Information retrieved successfully
   * } else {
   *     // Information retrieval failed
   * }
   * @endcode
   */
  _SC_EXTERN bool GetEdgeInfo(ScAddr const & edgeAddr, ScAddr & outSourceAddr, ScAddr & outTargetAddr) const
      noexcept(false);

  /*!
   * @brief Sets the content of an sc-link with a stream.
   *
   * This method sets the content of an sc-link identified by the given sc-address using the provided stream.
   *
   * @param addr The sc-address of the sc-link.
   * @param stream The stream containing the content.
   * @param isSearchableString Flag indicating whether the content is searchable as a string (default is true).
   * @return Returns true if the content was successfully set; otherwise, returns false.
   * @throws ExceptionInvalidParams if the specified sc-address or stream is invalid.
   * @throws ExceptionInvalidState if the file memory state is invalid.
   */
  _SC_EXTERN bool SetLinkContent(
      ScAddr const & addr,
      ScStreamPtr const & stream,
      bool isSearchableString = true) noexcept(false);

  /*!
   * @brief Sets the content of an sc-link with a typed string.
   *
   * This method sets the content of an sc-link identified by the given sc-address using the provided typed string.
   *
   * @param addr The sc-address of the sc-link.
   * @param stream The stream containing the content.
   * @param isSearchableString Flag indicating whether the content is searchable as a string (default is true).
   * @return Returns true if the content was successfully set; otherwise, returns false.
   * @throws ExceptionInvalidParams if the specified sc-address or stream is invalid.
   * @throws ExceptionInvalidState if the file memory state is invalid.
   *
   * @code
   * ScMemoryContext ctx;
   * ScAddr linkAddr = ctx.CreateLink(ScType::LinkConst);
   * if (ctx.SetLinkContent(linkAddr, "my node")) {
   *     // Content set successfully
   * } else {
   *     // Content set failed
   * }
   * @endcode
   *
   * @code
   * ScMemoryContext ctx;
   * ScAddr linkAddr = ctx.CreateLink(ScType::LinkConst);
   * if (ctx.SetLinkContent(linkAddr, 105)) {
   *     // Content set successfully
   * } else {
   *     // Content set failed
   * }
   * @endcode
   */
  template <typename TContentType>
  bool SetLinkContent(ScAddr const & addr, TContentType const & value, bool isSearchableString = true) noexcept(false)
  {
    return SetLinkContent(addr, ScStreamMakeRead(value), isSearchableString);
  }

  /*!
   * @brief Gets the content of an sc-link as a string.
   *
   * This method retrieves the content of an sc-link identified by the given sc-address as a string.
   *
   * @param addr The sc-address of the sc-link.
   * @param typedContent[out] The typed content retrieved from the sc-link.
   * @return Returns true if the content was successfully retrieved; otherwise, returns false.
   * @throws ExceptionInvalidParams if the specified sc-address is invalid.
   *
   * @code
   * ScMemoryContext ctx;
   * ScAddr linkAddr = ctx.CreateLink(ScType::LinkConst);
   * std::string outString;
   * if (ctx.GetLinkContent(linkAddr, outString)) {
   *     // Content get successfully
   * } else {
   *     // Content get failed
   * }
   * @endcode
   */
  _SC_EXTERN bool GetLinkContent(ScAddr const & addr, std::string & typedContent) noexcept(false)
  {
    ScStreamPtr const & ptr = GetLinkContent(addr);
    return ptr != nullptr && ptr->IsValid() && ScStreamConverter::StreamToString(ptr, typedContent);
  }

  /*!
   * @brief Gets the content of an sc-link as a stream.
   *
   * This method retrieves the content of an sc-link identified by the given sc-address as a stream.
   *
   * @param addr The sc-address of the sc-link.
   * @return Returns a shared pointer to the stream containing the content.
   * @throws ExceptionInvalidParams if the specified sc-address is invalid.
   * @throws ExceptionInvalidState if the file memory state is invalid.
   */
  _SC_EXTERN ScStreamPtr GetLinkContent(ScAddr const & addr) noexcept(false);

  /*!
   * @brief Gets the content of an sc-link as a typed string.
   *
   * This method retrieves the content of an sc-link identified by the given sc-address as a typed string.
   *
   * @param addr The sc-address of the sc-link.
   * @return Returns a shared pointer to the stream containing the content.
   * @throws ExceptionInvalidParams if the specified sc-address is invalid.
   * @throws ExceptionInvalidState if the file memory state is invalid.
   *
   * @code
   * ScMemoryContext ctx;
   * ScAddr linkAddr = ctx.CreateLink(ScType::LinkConst);
   * std::string outString;
   * if (ctx.GetLinkContent(linkAddr, outString)) {
   *     // Content get successfully
   * } else {
   *     // Content get failed
   * }
   * @endcode
   *
   * @code
   * ScMemoryContext ctx;
   * ScAddr linkAddr = ctx.CreateLink(ScType::LinkConst);
   * size_t outNumber;
   * if (ctx.GetLinkContent(linkAddr, outNumber)) {
   *     // Content get successfully
   * } else {
   *     // Content get failed
   * }
   * @endcode
   */
  template <typename TContentType>
  bool GetLinkContent(ScAddr const & addr, TContentType & typedContent) noexcept(false)
  {
    std::string content;
    ScStreamPtr const & ptr = GetLinkContent(addr);
    if (ptr != nullptr && ptr->IsValid() && ScStreamConverter::StreamToString(ptr, content))
    {
      std::istringstream streamString{content};
      streamString >> typedContent;

      return SC_TRUE;
    }

    return SC_FALSE;
  }

  /*!
   * @brief Finds sc-links by content using a stream.
   *
   * This method finds sc-links by matching the content with the provided stream.
   *
   * @param stream The stream to use for content matching.
   * @return Returns a vector of sc-addresses representing the found sc-links.
   * @throws ExceptionInvalidParams if the specified stream is invalid.
   * @throws ExceptionInvalidState if the file memory state is invalid.
   */
  _SC_EXTERN ScAddrVector FindLinksByContent(ScStreamPtr const & stream) noexcept(false);

  /*!
   * @brief Finds sc-links by content using a typed string.
   *
   * This method finds sc-links by matching the content with the provided typed string.
   *
   * @param stream The stream to use for content matching.
   * @return Returns a vector of sc-addresses representing the found sc-links.
   * @throws ExceptionInvalidParams if the specified stream is invalid.
   * @throws ExceptionInvalidState if the file memory state is invalid.
   *
   * @code
   * ScMemoryContext ctx;
   * ScAddrVector const & linkAddrsVector = ctx.FindLinksByContent("my node");
   * for (auto const & linkAddr : linkAddrsVector)
   * {
   *    // process links
   * }
   * @endcode
   */
  _SC_EXTERN template <typename TContentType>
  ScAddrVector FindLinksByContent(TContentType const & value) noexcept(false)
  {
    return FindLinksByContent(ScStreamMakeRead(value));
  }

  /*!
   * @brief Finds sc-links by content substring using a stream.
   *
   * This method finds sc-links by matching the content substring with the provided stream.
   *
   * @param stream The stream to use for content matching.
   * @param maxLengthToSearchAsPrefix The maximum length to search as a prefix (default is 0).
   * @return Returns a vector of sc-addresses representing the found sc-links.
   * @throws ExceptionInvalidParams if the specified stream is invalid.
   * @throws ExceptionInvalidState if the file memory state is invalid.
   *
   * @code
   * ScMemoryContext ctx;
   * ScAddrVector const & linkAddrsVector = ctx.FindLinksByContentSubstring("my");
   * for (auto const & linkAddr : linkAddrsVector)
   * {
   *    // process links
   * }
   * @endcode
   */
  _SC_EXTERN template <typename TContentType>
  ScAddrVector FindLinksByContentSubstring(TContentType const & value, size_t maxLengthToSearchAsPrefix = 0) noexcept(
      false)
  {
    return FindLinksByContentSubstring(ScStreamMakeRead(value), maxLengthToSearchAsPrefix);
  }

  /*!
   * @brief Finds sc-links by content substring using a typed string.
   *
   * This method finds sc-links by matching the content substring with the provided typed string.
   *
   * @param stream The stream to use for content matching.
   * @param maxLengthToSearchAsPrefix The maximum length to search as a prefix (default is 0).
   * @return Returns a vector of sc-addresses representing the found sc-links.
   * @throws ExceptionInvalidParams if the specified stream is invalid.
   * @throws ExceptionInvalidState if the file memory state is invalid.
   */
  _SC_EXTERN ScAddrVector
  FindLinksByContentSubstring(ScStreamPtr const & stream, size_t maxLengthToSearchAsPrefix = 0) noexcept(false);

  /*!
   * @brief Finds sc-links contents by content substring using a stream.
   *
   * This method finds sc-links contents by matching the content substring with the provided stream.
   *
   * @param stream The stream to use for content matching.
   * @param maxLengthToSearchAsPrefix The maximum length to search as a prefix (default is 0).
   * @return Returns a vector of strings representing the found sc-links contents.
   * @throws ExceptionInvalidParams if the specified stream is invalid.
   * @throws ExceptionInvalidState if the file memory state is invalid.
   *
   * @code
   * ScMemoryContext ctx;
   *  std::vector<std::string> const & linkContentVector = ctx.FindLinksByContentSubstring("my");
   * for (auto const & content : linkContentVector)
   * {
   *    // process contents
   * }
   * @endcode
   */
  _SC_EXTERN template <typename TContentType>
  std::vector<std::string> FindLinksContentsByContentSubstring(
      TContentType const & value,
      size_t maxLengthToSearchAsPrefix = 0) noexcept(false)
  {
    return FindLinksContentsByContentSubstring(ScStreamMakeRead(value), maxLengthToSearchAsPrefix);
  }

  /*!
   * @brief Finds sc-links contents by content substring using a typed string.
   *
   * This method finds sc-links contents by matching the content substring with the provided typed string.
   *
   * @param stream The stream to use for content matching.
   * @param maxLengthToSearchAsPrefix The maximum length to search as a prefix (default is 0).
   * @return Returns a vector of strings representing the found sc-links contents.
   * @throws ExceptionInvalidParams if the specified stream is invalid.
   * @throws ExceptionInvalidState if the file memory state is invalid.
   */
  _SC_EXTERN std::vector<std::string> FindLinksContentsByContentSubstring(
      ScStreamPtr const & stream,
      size_t maxLengthToSearchAsPrefix = 0) noexcept(false);

  /*!
   * @brief Saves the memory state.
   *
   * This method saves the state of the sc-memory.
   *
   * @return Returns true if the memory state was successfully saved; otherwise, returns false.
   */
  _SC_EXTERN bool Save();

  _SC_EXTERN template <
      typename ParamType1,
      typename ParamType2,
      typename ParamType3,
      typename ParamType4,
      typename ParamType5>
  std::shared_ptr<TIterator5<ParamType1, ParamType2, ParamType3, ParamType4, ParamType5>> Iterator5(
      ParamType1 const & param1,
      ParamType2 const & param2,
      ParamType3 const & param3,
      ParamType4 const & param4,
      ParamType5 const & param5)
  {
    return std::shared_ptr<TIterator5<ParamType1, ParamType2, ParamType3, ParamType4, ParamType5>>(
        new TIterator5<ParamType1, ParamType2, ParamType3, ParamType4, ParamType5>(
            *this, param1, param2, param3, param4, param5));
  }

  _SC_EXTERN template <typename ParamType1, typename ParamType2, typename ParamType3>
  std::shared_ptr<TIterator3<ParamType1, ParamType2, ParamType3>> Iterator3(
      ParamType1 const & param1,
      ParamType2 const & param2,
      ParamType3 const & param3)
  {
    return std::shared_ptr<TIterator3<ParamType1, ParamType2, ParamType3>>(
        new TIterator3<ParamType1, ParamType2, ParamType3>(*this, param1, param2, param3));
  }

  /*!
   * @brief Creates an iterator for iterating over triples.
   *
   * This method creates an iterator for iterating over triples and calls the provided function for each result.
   *
   * @tparam ParamType1 The type of the first parameter for the iterator.
   * @tparam ParamType2 The type of the second parameter for the iterator.
   * @tparam ParamType3 The type of the third parameter for the iterator.
   * @tparam FnT The type of the function to be called for each result.
   * @param param1 The first parameter for the iterator.
   * @param param2 The second parameter for the iterator.
   * @param param3 The third parameter for the iterator.
   * @param fn The function to be called for each result.
   *
   * @node fn function should have 3 parameters (ScAddr const & source, ScAddr const & edge, ScAddr const & target).
   */
  _SC_EXTERN template <typename ParamType1, typename ParamType2, typename ParamType3, typename FnT>
  void ForEachIter3(ParamType1 const & param1, ParamType2 const & param2, ParamType3 const & param3, FnT && fn)
  {
    ScIterator3Ptr it = Iterator3(param1, param2, param3);
    while (it->Next())
      fn(it->Get(0), it->Get(1), it->Get(2));
  }

  /*!
   * @brief Creates an iterator for iterating over triples.
   *
   * This method creates an iterator for iterating over triples and calls the provided function for each result.
   *
   * @tparam ParamType1 The type of the first parameter for the iterator.
   * @tparam ParamType2 The type of the second parameter for the iterator.
   * @tparam ParamType3 The type of the third parameter for the iterator.
   * @tparam ParamType4 The type of the fourth parameter for the iterator.
   * @tparam ParamType5 The type of the fifth parameter for the iterator.
   * @tparam FnT The type of the function to be called for each result.
   * @param param1 The first parameter for the iterator.
   * @param param2 The second parameter for the iterator.
   * @param param3 The third parameter for the iterator.
   * @param param4 The fourth parameter for the iterator.
   * @param param5 The fifth parameter for the iterator.
   * @param fn The function to be called for each result.
   *
   * @node fn function should have 4 parameters (ScAddr const & source, ScAddr const & edge, ScAddr const & target,
   * ScAddr const & attrEdge, ScAddr const & attr)
   */
  _SC_EXTERN template <
      typename ParamType1,
      typename ParamType2,
      typename ParamType3,
      typename ParamType4,
      typename ParamType5,
      typename FnT>
  void ForEachIter5(
      ParamType1 const & param1,
      ParamType2 const & param2,
      ParamType3 const & param3,
      ParamType4 const & param4,
      ParamType5 const & param5,
      FnT && fn)
  {
    ScIterator5Ptr it = Iterator5(param1, param2, param3, param4, param5);
    while (it->Next())
      fn(it->Get(0), it->Get(1), it->Get(2), it->Get(3), it->Get(4));
  }

  /*!
   * @brief Checks the existence of a directed edge between two sc-elements with the specified type.
   *
   * This function checks if there is an arc (directed edge) between the specified beginning and end sc-elements
   * with the given type. It returns true if the edge is found; otherwise, it returns false.
   *
   * @param begin The sc-address of the beginning sc-element.
   * @param end The sc-address of the ending sc-element.
   * @param edgeType The type of the directed edge to check.
   * @return Returns true if the specified edge exists; otherwise, returns false.
   * @throws ExceptionInvalidState if the sc-memory context is not valid.
   *
   * @code
   * ScMemoryContext ctx;
   * ScAddr node1 = ctx.CreateNode(ScType::NodeConst);
   * ScAddr node2 = ctx.CreateNode(ScType::NodeConst);
   * ScAddr edgeType = ctx.CreateNode(ScType::EdgeDCommonConst);
   * ctx.CreateEdge(node1, node2, edgeType);
   * bool hasEdge = ctx.HelperCheckEdge(node1, node2, edgeType);
   * @endcode
   */
  _SC_EXTERN bool HelperCheckEdge(ScAddr const & begin, ScAddr end, ScType const & edgeType) noexcept(false);

  /*!
   * @brief Resolves the sc-address of an sc-element by its system identifier.
   *
   * This function resolves the sc-address of an sc-element with the specified system identifier.
   * If the element is not found, it creates a new sc-node with the specified type and system identifier.
   *
   * @param sysIdtf The system identifier of the sc-element to resolve.
   * @param type The type of the sc-node to create if the element is not found.
   * @return Returns the sc-address of the resolved or created sc-element.
   * @throws ExceptionInvalidParams if the specified system identifier is invalid or the specified sc-address is invalid
   * or resolving sc-element type is not ScType::Node subtype
   * @throws ExceptionInvalidState if the sc-memory context is not valid or in an invalid state.
   *
   * @code
   * ScMemoryContext ctx;
   * ScAddr resolvedAddr = ctx.HelperResolveSystemIdtf("example_identifier", ScType::NodeConstClass);
   * @endcode
   */
  _SC_EXTERN ScAddr
  HelperResolveSystemIdtf(std::string const & sysIdtf, ScType const & type = ScType()) noexcept(false);

  /*!
   * @brief Resolves the sc-address of an sc-element by its system identifier.
   *
   * This function resolves the sc-address of an sc-element with the specified system identifier.
   * If the element is not found, it creates a new sc-node with the specified type and system identifier.
   * Additionally, it returns the system identifier fiver of the resolved or created sc-element.
   *
   * @param sysIdtf The system identifier of the sc-element to resolve.
   * @param type The type of the sc-node to create if the element is not found.
   * @param outFiver The 1st, 2d, 3d, 4th, 5th sc-element addresses of system identifier fiver of resolved
   *                              addr1 (resolved sc-element address)
   *                addr4           |
   *        addr5 --------------->  | addr2
   *     (nrel_system_identifier)   |
   *                              addr3 (system identifier sc-link)
   * @return Returns true if the sc-element is successfully resolved or created; otherwise, returns false.
   * @throws ExceptionInvalidParams if the specified system identifier is invalid or the specified sc-address is invalid
              or resolving sc-element type is not ScType::Node subtype.
   * @throws ExceptionInvalidState if the sc-memory context is not valid or in an invalid state.
   *
   * @code
   * ScMemoryContext ctx;
   * ScSystemIdentifierQuintuple resultFiver;
   * bool success = ctx.HelperResolveSystemIdtf("example_identifier", ScType::NodeConstClass, resultFiver);
   * @endcode
   */
  _SC_EXTERN bool HelperResolveSystemIdtf(
      std::string const & sysIdtf,
      ScType const & type,
      ScSystemIdentifierQuintuple & outFiver) noexcept(false);

  /*! Tries to set system identifier for sc-element ScAddr.
   * @param sysIdtf System identifier to set for sc-element `addr`
   * @param addr Sc-element address to set `sysIdtf` for it
   * @returns false if `sysIdtf` set for other sc-element address.
   * @throws utils::ExceptionInvalidParams if the specified sc-address is invalid
   * @throws ExceptionInvalidState if the sc-memory context is not valid or in an invalid state.
   *
   * @code
   * ScMemoryContext ctx;
   * ScAddr elementAddr = ctx.CreateNode(ScType::NodeConst);
   * bool success = ctx.HelperSetSystemIdtf("example_identifier", elementAddr);
   * @endcode
   */
  _SC_EXTERN bool HelperSetSystemIdtf(std::string const & sysIdtf, ScAddr const & addr) noexcept(false);

  /*! Tries to set system identifier for sc-element ScAddr.
   * @param sysIdtf System identifier to set for sc-element `addr`
   * @param addr Sc-element address to set `sysIdtf` for it
   * @param outFiver[out] The 1st, 2d, 3d, 4th, 5th sc-element addresses of system identifier fiver of sc-element `addr`
   * with set `sysIdtf`
   *                              addr1 (`addr`)
   *                addr4           |
   *        addr5 --------------->  | addr2
   *     (nrel_system_identifier)   |
   *                              addr3 (system identifier sc-link)
   * @returns false if `sysIdtf` set for other sc-element address.
   * @throws utils::ExceptionInvalidParams if the specified sc-address is invalid
   * @throws ExceptionInvalidState if the sc-memory context is not valid or in an invalid state.
   *
   * @code
   * ScMemoryContext ctx;
   * ScAddr elementAddr = ctx.CreateNode(ScType::NodeConst);
   * ScSystemIdentifierQuintuple resultFiver;
   * bool success = ctx.HelperSetSystemIdtf("example_identifier", elementAddr, resultFiver);
   * @endcode
   */
  _SC_EXTERN bool HelperSetSystemIdtf(
      std::string const & sysIdtf,
      ScAddr const & addr,
      ScSystemIdentifierQuintuple & resultFiver) noexcept(false);

  /*! Tries to get system identifier for sc-element ScAddr.
   * @param addr Sc-element address to get it system identifier
   * @returns "" if system identifier doesn't exist for `addr`.
   * @throws utils::ExceptionInvalidParams if the specified sc-address is invalid
   * @throws ExceptionInvalidState if the sc-memory context is not valid or in an invalid state.
   *
   * @code
   * ScMemoryContext ctx;
   * ScAddr elementAddr = ctx.CreateNode(ScType::NodeConst);
   * std::string sysIdtf = ctx.HelperGetSystemIdtf(elementAddr);
   * @endcode
   */
  _SC_EXTERN std::string HelperGetSystemIdtf(ScAddr const & addr) noexcept(false);

  /*!
   * @brief Finds an sc-element by its system identifier and returns its sc-address.
   *
   * This function searches for an sc-element with the specified system identifier and returns its sc-address.
   * If the element is found, the sc-address is stored in the 'outAddr' parameter, and the function returns true;
   * otherwise, it returns false.
   *
   * @param sysIdtf The system identifier of the sc-element to find.
   * @param outAddr A reference to store the sc-address of the found sc-element (if any).
   * @return Returns true if the sc-element is found; otherwise, returns false.
   * @throws ExceptionInvalidParams if the specified system identifier is invalid.
   * @throws ExceptionInvalidState if the sc-memory context is not valid or in an invalid state.
   *
   * @code
   * ScMemoryContext ctx;
   * ScAddr resultAddr;
   * bool found = ctx.HelperFindBySystemIdtf("example_identifier", resultAddr);
   * @endcode
   */
  _SC_EXTERN bool HelperFindBySystemIdtf(std::string const & sysIdtf, ScAddr & outAddr) noexcept(false);

  /*!
   * @brief Finds an sc-element by its system identifier and returns its sc-address.
   *
   * This function searches for an sc-element with the specified system identifier and returns its sc-address.
   * If the element is found, the function returns its sc-address; otherwise, it throws an exception.
   *
   * @param sysIdtf The system identifier of the sc-element to find.
   * @return Returns the sc-address of the found sc-element.
   * @throws ExceptionInvalidParams if the specified system identifier is invalid.
   * @throws ExceptionInvalidState if the sc-memory context is not valid or in an invalid state.
   *
   * @code
   * ScMemoryContext ctx;
   * ScAddr resultAddr = ctx.HelperFindBySystemIdtf("example_identifier");
   * @endcode
   */
  _SC_EXTERN ScAddr HelperFindBySystemIdtf(std::string const & sysIdtf) noexcept(false);

  /*!
   * @brief Finds an sc-element by its system identifier and returns its sc-address as a system identifier fiver.
   *
   * This function searches for an sc-element with the specified system identifier and returns its sc-address.
   * The sc-address is then converted into a system identifier fiver, which is stored in the 'outFiver' parameter.
   * If the element is found, the function returns true; otherwise, it returns false.
   *
   * @param sysIdtf The system identifier of the sc-element to find.
   * @param outFiver A reference to store the system identifier fiver of the found sc-element (if any).
   * @return Returns true if the sc-element is found; otherwise, returns false.
   * @throws ExceptionInvalidParams if the specified system identifier is invalid.
   * @throws ExceptionInvalidState if the sc-memory context is not valid or in an invalid state.
   *
   * @code
   * ScMemoryContext ctx;
   * ScSystemIdentifierQuintuple resultFiver;
   * bool found = ctx.HelperFindBySystemIdtf("example_identifier", resultFiver);
   * @endcode
   */
  _SC_EXTERN bool HelperFindBySystemIdtf(std::string const & sysIdtf, ScSystemIdentifierQuintuple & outFiver) noexcept(
      false);

  /*!
   * Generates sc-constructions by isomorphic sc-template and accumulates generated sc-construction into `result`.
   * @param templ A sc-template object to find constructions by it.
   * @param result A generated sc-construction.
   * @param params A map of specified sc-template sc-variables to user replacements.
   * @param resultCode A pointer to status of method completion.
   * @return Returns true if the construction is generated; otherwise, returns false. It is the same as `resultCode`.
   * @throws utils::ExceptionInvalidState if sc-template is not valid
   *
   * @code
   * ...
   * ...
   * ScTemplate templ;
   * templ.Triple(
   *  classAddr,
   *  ScType::EdgeAccessVarPosPerm >> "_edge",
   *  ScType::Unknown >> "_addr2"
   * );
   *
   * ScTemplateResultItem result;
   * m_ctx->HelperGenTemplate(templ, result);
   *
   * // handle generated sc-construction sc-elements
   * m_ctx->IsElement(item["_addr2"])
   * @endcode
   */
  _SC_EXTERN ScTemplate::Result HelperGenTemplate(
      ScTemplate const & templ,
      ScTemplateResultItem & result,
      ScTemplateParams const & params = ScTemplateParams::Empty,
      ScTemplateResultCode * resultCode = nullptr) noexcept(false);

  /*!
   * Searches sc-constructions by isomorphic sc-template and accumulates found sc-constructions into `result`.
   * @param templ A sc-template object to find sc-constructions by it.
   * @param result A result vector of found sc-constructions.
   * @return Returns true if the sc-constructions are found; otherwise, returns false.
   * @throws utils::ExceptionInvalidState if sc-template is not valid
   *
   * @note Use this method if expected searchable sc-constructions vector is not big. If it is very big, please, use one
   * of callback-based HelperSearchTemplate.
   *
   * @code
   * ...
   * ...
   * ScTemplate templ;
   * templ.Triple(
   *  classAddr,
   *  ScType::EdgeAccessVarPosPerm >> "_edge",
   *  ScType::Unknown >> "_addr2"
   * );
   *
   * ScTemplateSearchResult result;
   * m_ctx->HelperSearchTemplate(templ, result);
   *
   * // iterate by all result sc-constructions
   * for (size_t i = 0; i < result.Size(); ++i)
   * {
   *   ScTemplateResultItem const & item = result[i];
   *   // handle each result sc-construction sc-elements
   *   m_ctx->IsElement(item["_addr2"])
   * }
   * @endcode
   */
  _SC_EXTERN ScTemplate::Result HelperSearchTemplate(
      ScTemplate const & templ,
      ScTemplateSearchResult & result) noexcept(false);

  /*!
   * Searches sc-constructions by isomorphic sc-template and passes found sc-constructions to `callback`
   * lambda-function. If `filterCallback` passed, then all found constructions triples are filtered by `filterCallback`
   * condition.
   * @param templ A sc-template object to find sc-constructions by it
   * @param callback A lambda-function, callable when each sc-construction triple was found
   * @param filterCallback A lambda-function, that filters all found sc-constructions triples
   * @param checkCallback A lambda-function, that filters all found triples with checking sc-address
   * @return Returns true if the sc-constructions are found; otherwise, returns false.
   * @throws utils::ExceptionInvalidState if sc-template is not valid
   *
   * @code
   * ...
   * ...
   * ScAddr const & structureAddr = ctx.CreateNode(ScType::NodeConstStruct);
   * ScAddr const & modelAddr = ctx.CreateNode(ScType::NodeConstStruct);
   * ...
   * ScAddr const & setAddr = ctx.CreateNode(ScType::NodeConst);
   * ScTemplate templ;
   * templ.Triple(
   *  classAddr,
   *  ScType::EdgeAccessVarPosPerm >> "_edge",
   *  ScType::Unknown >> "_addr2"
   * );
   * m_ctx->HelperSearchTemplate(templ, [&ctx](ScTemplateSearchResultItem const & item) {
   *  ctx.CreateEdge(ScType::EdgeAccessConstPosTemp, setAddr, item["_addr2"]);
   * }, [&ctx](ScTemplateSearchResultItem const & item) -> bool {
   *  return !ctx->HelperCheckEdge(structureAddr, item["_edge"], ScType::EdgeAccessConstPosPerm);
   * }, [&ctx](ScAddr const & addr) -> bool {
   *  return ctx->HelperCheckEdge(modelAddr, addr, ScType::EdgeAccessConstPosPerm);
   * });
   * @endcode
   */
  _SC_EXTERN void HelperSearchTemplate(
      ScTemplate const & templ,
      ScTemplateSearchResultCallback const & callback,
      ScTemplateSearchResultFilterCallback const & filterCallback = {},
      ScTemplateSearchResultCheckCallback const & checkCallback = {}) noexcept(false);

  _SC_EXTERN void HelperSearchTemplate(
      ScTemplate const & templ,
      ScTemplateSearchResultCallback const & callback,
      ScTemplateSearchResultCheckCallback const & checkCallback) noexcept(false);

  /*!
   * Searches constructions by isomorphic sc-template and pass found sc-constructions to `callback`
   * lambda-function. Lambda-function `callback` must return a request command value to manage sc-template search:
   *  - ScTemplateSearchRequest::CONTINUE,
   *  - ScTemplateSearchRequest::STOP,
   *  - ScTemplateSearchRequest::ERROR.
   * When ScTemplateSearchRequest::CONTINUE returns, sc-template search will be continued. If
   * ScTemplateSearchRequest::STOP or ScTemplateSearchRequest::ERROR returns, then sc-template search stops.
   * If sc-template search stopped by ScTemplateSearchRequest::ERROR, then HelperSmartSearchTemplate thrown
   * utils::ExceptionInvalidState.
   * If `filterCallback` passed, then all found sc-constructions triples are filtered by `filterCallback` condition.
   * @param templ A sc-template object to find sc-constructions by it
   * @param callback A lambda-function, callable when each sc-construction triple was found
   * @param filterCallback A lambda-function, that filters all found sc-constructions triples
   * @param checkCallback A lambda-function, that filters all found triples with checking sc-address
   * @return Returns true if the sc-constructions are found; otherwise, returns false.
   * @throws utils::ExceptionInvalidState if sc-template is not valid
   *
   * @code
   * ...
   * ...
   * ScAddr const & structureAddr = ctx.CreateNode(ScType::NodeConstStruct);
   * ...
   * ScAddr const & setAddr = ctx.CreateNode(ScType::NodeConst);
   * ScTemplate templ;
   * templ.Triple(
   *  classAddr,
   *  ScType::EdgeAccessVarPosPerm >> "_edge",
   *  ScType::Unknown >> "_addr2"
   * );
   * m_ctx->HelperSmartSearchTemplate(templ, [&ctx](ScTemplateSearchResultItem const & item) -> ScTemplateSearchRequest
   * {
   *   ScAddr const & edgeAddr = item["_edge"];
   *   if (ctx->HelperCheckEdge(structureAddr, edgeAddr, ScType::EdgeAccessConstPosPerm))
   *    return ScTemplateSearchRequest::CONTINUE;
   *
   *   if (ctx.CreateEdge(ScType::EdgeAccessConstPosTemp, setAddr, item["_addr2"]))
   *    return ScTemplateSearchRequest::STOP;
   *
   *   return ScTemplateSearchRequest::ERROR;
   * });
   * @endcode
   */
  _SC_EXTERN void HelperSmartSearchTemplate(
      ScTemplate const & templ,
      ScTemplateSearchResultCallbackWithRequest const & callback,
      ScTemplateSearchResultFilterCallback const & filterCallback = {},
      ScTemplateSearchResultCheckCallback const & checkCallback = {}) noexcept(false);

  _SC_EXTERN void HelperSmartSearchTemplate(
      ScTemplate const & templ,
      ScTemplateSearchResultCallbackWithRequest const & callback,
      ScTemplateSearchResultCheckCallback const & checkCallback) noexcept(false);

  SC_DEPRECATED(
      0.8.0,
      "Use callback-based ScMemoryContext::HelperSearchTemplate(ScTemplate const & templ, "
      "ScTemplateSearchResultCallback const & callback, ScTemplateSearchResultCheckCallback const & checkCallback) "
      "instead.")
  _SC_EXTERN ScTemplate::Result HelperSearchTemplateInStruct(
      ScTemplate const & templ,
      ScAddr const & scStruct,
      ScTemplateSearchResult & result) noexcept(false);

  /*!
   * Builds a program object of isomorphic template from existing in sc-memory sc-address of sc-structure. After
   * sc-template built you can use it to search or generate sc-constructions.
   * @param templ A built program object of isomorphic template.
   * @param templAddr A sc-address of sc-template structure.
   * @param params A map of specified sc-template sc-variables to user replacements.
   * @return Returns true if the sc-template is built; otherwise, returns false.
   * @throws utils::ExceptionInvalidState if sc-template is not valid
   *
   * @code
   * ...
   * ...
   * ScTemplate templ;
   * ScAddr const & templAddr = m_ctx->HelperFindBySystemIdtf("my_template");
   * m_ctx->HelperBuildTemplate(templ, templAddr);
   * ...
   * @endcode
   */
  _SC_EXTERN ScTemplate::Result HelperBuildTemplate(
      ScTemplate & templ,
      ScAddr const & templAddr,
      ScTemplateParams const & params = ScTemplateParams()) noexcept(false);

  /*!
   * Builds a program object of isomorphic template represented in sc.s-text. After sc-template built you can use it to
   * search or generate sc-constructions.
   * @param templ A built program object of isomorphic template.
   * @param scsText A sc.s-representation of isomorphic template.
   * @return Returns true if the sc-template is built; otherwise, returns false.
   * @throws utils::ExceptionInvalidState if sc-template is not valid
   *
   * @code
   * ...
   * ...
   * ScTemplate templ;
   * std::string const scsTempl = "concept_set _-> _var;;";
   * m_ctx->HelperBuildTemplate(templ, scsTempl);
   * ...
   * @endcode
   */
  _SC_EXTERN ScTemplate::Result HelperBuildTemplate(ScTemplate & templ, std::string const & scsText) noexcept(false);

  _SC_EXTERN ScMemoryStatistics CalculateStat() const;

private:
  sc_memory_context * m_context;
  std::string m_name;
};

class ScMemoryContextEventsPendingGuard
{
public:
  _SC_EXTERN explicit ScMemoryContextEventsPendingGuard(ScMemoryContext & ctx)
    : m_ctx(ctx)
  {
    m_ctx.BeingEventsPending();
  }

  _SC_EXTERN ~ScMemoryContextEventsPendingGuard()
  {
    m_ctx.EndEventsPending();
  }

private:
  ScMemoryContext & m_ctx;
};
