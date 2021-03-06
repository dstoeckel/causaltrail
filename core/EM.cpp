#include "EM.h"

EM::EM(Network& network, Matrix<int>& observations, float difference,
       unsigned int runs)
    : network_(network),
      observations_(observations),
      probHandler_(network),
      differenceThreshold_(difference),
      maxRuns_(runs)
{
	performEM();
}

void EM::performEM()
{
	start = std::chrono::system_clock::now();
	// Check completness of the data
	if(observations_.contains(-1)) {
		// Determine the best initialization method
		method_ = getMaxMethod_();
		// Perform another EM run with the best method.
		std::tie(finalDifference_, neededRuns_) = runEMIterations_();
	} else {
		// Calculate parameters directly
		finalDifference_ = mPhase();
		neededRuns_ = 1;
	}
	end = std::chrono::system_clock::now();
}

unsigned int EM::getMaxMethod_()
{
	float maxprob = 0.0f;
	unsigned int maxmethod = 0;

	for(unsigned int method = 0; method < 2; method++) {
		// Initialize and iterate E/M-Phase
		method_ = method;
		runEMIterations_();
		float prob = calculateLikelihoodOfTheData();
		if(prob > maxprob) {
			maxprob = prob;
			maxmethod = method;
		}
	}

	return maxmethod;
}

std::pair<float, unsigned int> EM::runEMIterations_()
{
	unsigned int runs = 0;
	float difference = std::numeric_limits<float>::infinity();

	initalise();
	while(difference > differenceThreshold_ && runs < maxRuns_) {
		ePhase();
		difference = mPhase();
		runs++;
	}

	return std::make_pair(difference, runs);
}

float EM::calculateProbabilityEM(Node& n, unsigned int col, unsigned int row)
{
	// get Parents
	auto ParentIDs = n.getParents();
	const Matrix<float>& probMatrix = n.getProbabilityMatrix();

	// compute TotProbParentsRec
	float totProbParents = 1.0f;
	for(unsigned int key = 0; key < n.getNumberOfParents(); key++) {
		totProbParents *= probHandler_.computeTotalProbability(
		    ParentIDs[key], network_.reverseFactor(n, key,row));
	}

	// computeNormalizingProb
	float denominator = 0.0f;
	for(unsigned int col = 0; col < probMatrix.getColCount(); col++) {
		denominator += probMatrix(col, row) * totProbParents;
	}

	// return result
	float nominator = probMatrix(col - 1, row) * totProbParents;
	return nominator / denominator;
}

void EM::calculateExpectedValue(unsigned int row, Node& n)
{
	Matrix<int>& obMatrix = n.getObservationMatrix();	
	if(obMatrix.hasNACol()) {
		for(unsigned int col = 1; col < obMatrix.getColCount(); col++) {
			float value =
			    obMatrix(col, row) +
			    calculateProbabilityEM(n, col, row) * obMatrix(0, row);
			obMatrix.setData(value, col, row);
		}
	}
}

void EM::ePhase()
{
	for(auto& n : network_.getNodes()) {
		for(unsigned int row = 0; row < n.getNumberOfParentValues(); row++) {
			calculateExpectedValue(row, n);
		}
	}
}

void EM::calculateMaximumLikelihood(unsigned int row, unsigned int& counter,
                                    float& difference, Node& n,
                                    const Matrix<int>& obMatrix)
{
	float rowsum = obMatrix.calculateRowSum(row);
	if(obMatrix.hasNACol()){
			for(unsigned int col = 1; col < obMatrix.getColCount(); col++) {
				float probability = 0.0f;
				if ((rowsum-obMatrix(0,row)) > 0.0){
					probability = obMatrix(col, row) /
			    	                (rowsum - obMatrix(0, row));
					difference += fabs(n.getProbability(col - 1, row) - probability);
				}
				n.setProbability(probability, col - 1, row);
				counter++;
				}
			}
	else {
			for(unsigned int col = 0; col < obMatrix.getColCount(); col++) {
				float probability =  0.0f;
				if (rowsum != 0){
					probability = obMatrix(col, row) / rowsum;
					difference += fabs(n.getProbability(col, row) - probability);
				}
				n.setProbability(probability, col, row);
				counter++;
			}
	}
}

float EM::mPhase()
{
	float difference = 0.0f;
	unsigned int counter = 0;
	for(auto& n : network_.getNodes()) {
		const Matrix<int>& obMatrix = n.getObservationMatrix();
		for(unsigned int row = 0; row < obMatrix.getRowCount(); row++)
			calculateMaximumLikelihood(row, counter, difference, n, obMatrix);
		n.loadBackup();
	}
	return difference / counter;
}


void EM::initalise()
{
	switch(method_) {
		case 0:
			initaliseAssumingUniformDistribution();
			break;
		case 1:
			initaliseAccordingToInitialDistribution();
			break;
	}
}

void EM::initaliseAssumingUniformDistribution()
{
	for(auto& n : network_.getNodes()) {
		const Matrix<float>& probMatrix = n.getProbabilityMatrix();
		for(unsigned int row = 0; row < probMatrix.getRowCount(); row++) {
			for(unsigned int col = 0; col < probMatrix.getColCount(); col++) {
				n.setProbability(1.0f / n.getNumberOfUniqueValuesExcludingNA(),
				                 col, row);
			}
		}
	}
}

void EM::initaliseAccordingToInitialDistribution()
{
	for(auto& n : network_.getNodes()) {
		const Matrix<int>& obMatrix = n.getObservationMatrix();
		const Matrix<float>& probMatrix = n.getProbabilityMatrix();
		for(unsigned int row = 0; row < probMatrix.getRowCount(); row++) {
			float rowsum = obMatrix.calculateRowSum(row);
			for(unsigned int col = 0; col < probMatrix.getColCount(); col++) {
				if(obMatrix.hasNACol()) {
					n.setProbability(
					    n.getObservationMatrix()(col + 1, row) /
					        (rowsum - n.getObservationMatrix()(0, row)),
					    col, row);
				} else {
					n.setProbability(
					    n.getObservationMatrix()(col, row) / rowsum, col, row);
				}
			}
		}
	}
}

float EM::calculateLikelihoodOfTheData()
{
	return probHandler_.calculateLikelihoodOfTheData(observations_);
}


int EM::getNumberOfRuns(){
	return neededRuns_;
}


float EM::getDifference(){
	return finalDifference_;
}

int EM::getTimeInMicroSeconds(){
	return std::chrono::duration_cast<std::chrono::microseconds>
                             (end-start).count();
}	
