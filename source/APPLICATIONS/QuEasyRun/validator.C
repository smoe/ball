/* validator.C
 * 
 * Copyright (C) 2009 Marcel Schumann
 * 
 * This file is part of QuEasy -- A Toolbox for Automated QSAR Model
 * Construction and Validation.
 * QuEasy is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or (at
 * your option) any later version.
 * 
 * QuEasy is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 */

#include <fstream>
#include <BALL/QSAR/registry.h>
#include <BALL/QSAR/featureSelection.h>
#include <BALL/QSAR/configIO.h>

using namespace BALL::QSAR;
using namespace BALL;


void startValidation(ifstream& in, QSARData* q, String* data_filename)
{
	ValidationConfiguration conf = ConfigIO::readValidationConfiguration(&in);
		
	if(conf.done || conf.for_visualization_only==1) return ; // stop processing this section and continue with next section
		
	bool created_data_object=0;
	if(q==NULL || data_filename==NULL)
	{
		q = new QSARData;
		created_data_object=1;
	}	
	
	Registry reg;
	Model* m;
	String model_type;
		
	ifstream model_input(conf.model.c_str()); // read model-abbreviation
	if(!model_input)
	{
		cout<<"Error: Model-file '"<<conf.model<<"' does not exist!!"<<endl;
		return;
	}
	getline(model_input,model_type);
	getline(model_input,model_type);
	model_type = model_type.getField(0,"\t");
	model_input.close();
		
	RegistryEntry* entry = reg.getEntry(model_type);
	bool regression = entry->regression;
					
	if(!entry->kernel)
	{
		m = (*entry->create)(*q);
	}
	else
	{	
		// parameters irrelevant; will be overwritten by those read from file
		m = (*entry->createKernel1)(*q,1,1, -1);
	}
			
	m->readFromFile(conf.model.c_str());
	m->model_val->selectStat(conf.statistic);
		
	if(conf.data!="")
	{
		if(!data_filename || conf.data!=*data_filename)
		{
			q->readFromFile(conf.data);
			*data_filename = conf.data;
		}		
		
		if(conf.val_type==1) m->model_val->testInputData(1);
			
		else if(conf.val_type==2) m->model_val->crossValidation(conf.k_folds,1);
		
		else if(conf.val_type==3) m->model_val->bootstrap(conf.bootstrap_samples);
		
		else if(conf.val_type==4) m->model_val->yRandomizationTest(conf.no_of_permutation_tests,conf.k_folds);
		
		else if(conf.val_type==6)
		{
			if(entry->regression)
			{
				((RegressionModel*)m)->validation->calculateCoefficientStdErrors(conf.bootstrap_samples);
			}
		}
		
		else if(conf.val_type==7)
		{
			if(!data_filename || conf.validation_data!=*data_filename)
			{
				q->readFromFile(conf.validation_data.c_str());
				*data_filename = conf.validation_data;
			}	
			m->model_val->testInputData(1);
		}				
	}
	
	// save the result of the validation to the specified file
	m->model_val->saveToFile(conf.output);
	
	if(created_data_object) delete q;
	delete m;
}


#ifndef EXT_MAIN
int main(int argc, char* argv[])
{ 
	if(argc<2)
	{
		cout<<"Please specify configuration file!"<<endl; 
		return 0;
	}
	
	ifstream in(argv[1]);
	if(!in)
	{
		cout<<"Configuration file '"<<argv[1]<<"' not found!"<<endl;
		return 0;
	}
	
	String line;
	
	for(int i=0;!in.eof();i++) // read ALL Validator sections
	{
		for(int j=0;!in.eof();j++) // skip everthing until the beginning of the next Validator-section
		{
			getline(in,line);
			if(!line.hasPrefix("[Validator]")) continue;
			else break;
		}
		if(!line.hasPrefix("[Validator]")) break; // there are no (more) Validator-sections!
		ConfigIO::putbackLine(&in,line);
		
		startValidation(in,NULL,NULL);		
	}
}
#endif