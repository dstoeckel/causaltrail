#ifndef QUERYEXECUTER
#define QUERYEXECUTER
#include "ProbabilityHandler.h"
#include "Interventions.h"
#include "NetworkController.h"

class QueryExecuter{

	public:
	/**
	 * @param c A reference to a NetworkController
	 */
	explicit QueryExecuter(NetworkController& c);

	QueryExecuter(const QueryExecuter& o)
		: networkController_(o.networkController_),
		  probHandler_(o.probHandler_),
		  interventions_(o.interventions_),
		  nonInterventionNodeID_(o.nonInterventionNodeID_),
		  nonInterventionValues_(o.nonInterventionValues_),
		  conditionNodeID_(o.conditionNodeID_),
		  conditionValues_(o.conditionValues_),
		  doInterventionNodeID_(o.doInterventionNodeID_),
		  doInterventionValues_(o.doInterventionValues_),
		  addEdgeNodeIDs_(o.addEdgeNodeIDs_),
		  removeEdgeNodeIDs_(o.removeEdgeNodeIDs_),
		  argmaxNodeIDs_(o.argmaxNodeIDs_)
	{
	}

	QueryExecuter& operator=(const QueryExecuter&) = delete;
	QueryExecuter& operator=(QueryExecuter&&) = delete;

	/**QueryExecuter
	 * 
	 * @return pair of 
	 * (1) probability 
	 * (2) value assignments (only for MAP queries)
	 *
	 */
	std::pair<float,std::vector<std::string>> execute();

	/**
	 * Stores a pair of nodeID and value reflecting a nonIntervention
	 *
	 * @param nodeID identifier of the node to be stored
	 * @param value value to be stored
	 *
	 */
	 void setNonIntervention(const unsigned int nodeID, const unsigned int value);

	/**
	 * Stores a pair of nodeID and value reflecting a Condition
	 *
	 * @param nodeID Identifier of the node to be stored.
	 * @param value Value to be stored.
	 */
	void setCondition(const unsigned int nodeID,const unsigned int value);

	/**setDoIntervention
	 * 
	 * @param nodeID, identifier of the node to be stored
	 * @param value, value to be stored 
	 *
	 * Stores a pair of nodeID and value reflecting a Do-Intervention
	 */
	void setDoIntervention(const unsigned int nodeID, const unsigned int value);

	/**setAddEdge
	 * 
	 * @param nodeID1, identifier of the source edge
	 * @param nodeID2, identifier of the target edge
	 *
	 * Stores a pair of two node identifiers reflecting an edge addition
	 */
	void setAddEdge(const unsigned int nodeID1, const unsigned int nodeID2);

	/**setRemoveEdge
	 * 
	 * @param nodeID1, identifier of the source edge
	 * @param nodeID2, identifier of the target edge
	 *
	 * Stores a pair of two node identifiers reflecting an edge deletion
	 */
	void setRemoveEdge(const unsigned int nodeID1, const unsigned int nodeID2);

	/**setArgMax
	 * 
	 * @param nodeID1, identifier of the source edge
	 * 
	 * Stores a nodeID, which is part of a MAP query
	 */
	void setArgMax(const unsigned int nodeID);

	const std::vector<unsigned int>& getNonInterventionIds() const;
	const std::vector<int>& getNonInterventionValues() const;

	const std::vector<unsigned int>& getInterventionIds() const;
	const std::vector<int>& getInterventionValues() const;

	const std::vector<unsigned int>& getConditionIds() const;
	const std::vector<int>& getConditionValues() const;

	const std::vector<std::pair<unsigned int, unsigned int>>& getEdgeRemovalIds() const;
	const std::vector<std::pair<unsigned int, unsigned int>>& getEdgeAdditionIds() const;
	const std::vector<unsigned int>& getArgMaxIds() const;

	/**operator<<
	 * 
	 * @param os, reference to an ostream object
	 * @param qe, const reference to a QueryExecuter object
	 *
	 * @return an ostream reference
	 *
	 */
	friend std::ostream& operator<< (std::ostream& os, const QueryExecuter& qe);

	private:

	/**isCOunterfactual
	 *
	 * @return true if the given query represents a counterfactual, false otherwise
	 */
	bool isCounterfactual();


	/**adaptNodeIdentifiers
	 *
	 *Adapts the node identifiers to the hypothetical Nodes of the Twin Network
	 */
	void adaptNodeIdentifiers();

	/**hasInterventions
	 * 
	 * @return true if a query contains interventions, false otherwise
	 *
	 */
	bool hasInterventions();

	/**executeInterventions
	 * 
	 * Depending on the typ of intervention, the suitable
	 * specialised methods are called to execute them 
	 */
	void executeInterventions();

	/**reverseInterventions
	 * 
	 * Depending on the typ of intervention, the suitable
	 * specialised methods are called to reverse them
	 */
	void reverseInterventions();

	/**computeProbability
	 * 
	 * @return a pair of the resulting probability and optinal value assignments (for MAP queries)
	 *
	 * Depending on the typ of probability that is to comput, this method calls
	 * the suitable specialised methods. The second member of the pair is a empty string vector,
	 * except for MAP queries.
	 */
	std::pair<float,std::vector<std::string>> computeProbability();

	/**executeDoInterventions
	 * 
	 * Calls the appropriate methods in the Intervention object
	 * to perform all Do-Interventions
	 */
	void executeDoInterventions();

	/**executeReverseDoInterventions
	 * 
	 * Calls the appropriate methods in the Intervention object
	 * to reverse all do interventions
	 */
	void executeReverseDoInterventions();

	/**executeEdgeAdditions
	 * 
	 * Calls the appropriate methods in the Intervention object
	 * to add all specified edges
	 */
	void executeEdgeAdditions();

	/**executeEdgeDeletions
	 * 
	 * Calls the appropriate methods in the Intervention object 
	 * to remove all specified edges
	 */
	void executeEdgeDeletions();

	/**executeEdgeAddditionsReverse
	 * 
	 * Reverses all edge additions
	 */
	void executeEdgeAdditionsReverse();

	/**executeEdgeDeletionsReverse
	 * 
	 * Reverses all edge deletions
	 */
	void executeEdgeDeletionsReverse();	

	/**executeArgMax
	 * 
	 * @return a pair of the maximum probability and the value assignment
	 *
	 * Uses the ProbabilityHandler to compute a MAP query
	 */
	std::pair<float,std::vector<std::string>> executeArgMax();

	/**executeCondition
	 * 
	 * @return the probability of the condition
	 *
	 * Uses the ProbabilityHandler to compute a conditional probability
	 */
	float executeCondition();

	/**executeProbability
	 *
	 * @return the probability of the query
	 *
	 * Uses the ProbabilityHandler to compute a total-/ or joint probability
	 */
	float executeProbability();

	//Reference to the network controller
	NetworkController& networkController_;	
	//Instance of a ProbabilityHandler class to calculate the requested probabilities
	ProbabilityHandler probHandler_;
	//Instance of a Interventions class to perform the interventions
	Interventions interventions_;
	//vectors to store node identifiers and values in the category related to their occurence in the user query
	std::vector<unsigned int> nonInterventionNodeID_;
	std::vector<int> nonInterventionValues_;
	std::vector<unsigned int> conditionNodeID_;
	std::vector<int> conditionValues_;
	std::vector<unsigned int> doInterventionNodeID_;
	std::vector<int> doInterventionValues_;
	std::vector<std::pair<unsigned int, unsigned int>> addEdgeNodeIDs_;
	std::vector<std::pair<unsigned int, unsigned int>> removeEdgeNodeIDs_;
	std::vector<unsigned int> argmaxNodeIDs_;
};

#endif
