// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
//

#ifndef LVMODEL
#define LVMODEL

#ifndef BALL_LINALG_MATRIX_H
#include <BALL/MATHS/LINALG/matrix.h>
#endif


namespace BALL
{
	namespace QSAR 
	{
		/** Interface-class for all latente variable models (PLS, PCR, ...). 
		It is used to store and access the latente variables (PLS/PCR components), the loading matrix and the weight matrix in the same way for all such models.*/
		class LatentVariableModel
		{ 
			public:
				LatentVariableModel();				
				
				/** Return the latent variable matrix as computed by the last training.\n
				It contains one loading vector in each column. Each loading vector contains the importance of each feature for a latente variable. */
				const Matrix<double>* getLatentVariables();
				
				/** Return the weights matrix as computed by the last training.\n
				It contains one weight vector in each column. Each weight vector contains the importance of each latente variable for modelling the result. */
				const Matrix<double>* getWeights();
				
				/** Return the loading matrix as computed by the last training.\n
				It contains one loading vector in each column. Each loading vector contains the importance of each feature for a latente variable. */
				const Matrix<double>* getLoadings();
			
			protected:
				
				/** The loading-matrix which contains one loading vector in each column. Each loading vector contains the importance of each feature for a latente variable. */
				Matrix<double> latent_variables_;
				
				/** The loading-matrix which contains one loading vector in each column. Each loading vector contains the importance of each feature for a latente variable. */
				Matrix<double> loadings_;
				
				/** The weights-matrix which contains one weight vector in each column. Each weight vector contains the importance of each latente variable for modelling the result. */
				Matrix<double> weights_;
				
				friend class RegressionValidation;	
		};
	}
}

#endif // LVMODEL
