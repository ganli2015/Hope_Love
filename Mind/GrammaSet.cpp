#include "StdAfx.h"
#include "GrammaSet.h"
#include "FilePath.h"
#include "GrammarFeatureModel.h"
#include "GrammarLocalModel.h"
#include "CommonFunction.h"

#include "../CommonTools/LogWriter.h"

#include "../MindElement/GrammarLocal.h"

#include "../DataCollection/LanguageFunc.h"
#include "../DataCollection/Word.h"

#include "../CommonTools/ConfigureInfoManager.h"
#include "../CommonTools/GeneralFunctor.h"
#include "../CommonTools/CommonStringFunction.h"

#include "../Mathmatic/FindSequence.h"

#include <tinyxml.h>

#include <fstream>
#include <functional>
#include <numeric>

using namespace DataCollection;
using namespace std;
using namespace Math;


namespace Mind
{
	GrammarSet::GrammarSet(void):_featureModel(shared_ptr<GrammarFeatureModel>(new GrammarFeatureModel()))
	{
		Initialize();
		LOG("Initialized GrammarSet");
	}		



	GrammarSet::~GrammarSet(void)
	{
	}

	void GrammarSet::InitializeGrammarModel()
	{
		_featureModel = make_shared<GrammarFeatureModel>();

		_localModel = make_shared<GrammarLocalModel>();
		_localModel->ReadGrammarLocal();

		_patternModel = make_shared<GrammarPatternModel>(this);
	}

	void GrammarSet::Initialize()
	{
#ifdef _Extract_Initial_Grammar_Pattern
		ExtractGrammarPatternFromInitialFile();
#endif // _Extract_Initial_Grammar_Pattern


		//Input the grammar patterns from the file.
		vector<GrammarAttribute> patterns=InputGrammaPatterns(GetHopeLoveMindPath()+GrammaPatterns_InitialFilename);

		//Build the grammar trees.
		for (int i=0;i<DataCollection::NUM_PARTOFSPEECH;++i)
		{
			GrammaTree for_tree(i);
			GrammaTree back_tree(i);
			_forwardtree.insert(make_pair(i,for_tree));
			_backwardtree.insert(make_pair(i,back_tree));
		}

		//Add patterns to <me>.
		for (size_t i=0;i<patterns.size();++i)
		{
			GrammarPattern pattern=patterns[i].pattern;
			_patterns.push_back(patterns[i]);

			//AddPatternToTree(pattern);
		}

		ReadWeights();

		CFG_SECTION(COMPUTE_GRAMMAR_WEIGHTS)
		{
			InitializeWeights();
		}
	}

	vector<GrammarAttribute> GrammarSet::InputGrammaPatterns( std::string filename )
	{
		vector<GrammarAttribute> patterns;

		ifstream in(filename);
		if (!in) return patterns;

		int id(0);
		string line = "";
		while(getline(in,line))
		{
// 			int count;
// 			in>>count;
// 
// 			vector<int> pattern_int;
// 			pattern_int.reserve(count);
// 			for (int i=1;i<=count;++i)
// 			{
// 				int elem;
// 				in>>elem;
// 				out << elem<<" ";
// 				pattern_int.push_back(elem);
// 			}
// 
// 
// 			vector<PartOfSpeech> pattern_enum;
// 			pattern_enum.reserve(pattern_int.size());
// 			for (size_t i=0;i<pattern_int.size();++i)
// 			{
// 				pattern_enum.push_back((PartOfSpeech)pattern_int[i]);
// 			}
// 
// 			GrammarPattern pattern(pattern_enum);
// 			pattern.SetID(id++);
// 			int frequency;
// 			in>>frequency;
// 
// 			out << frequency<<endl;

			vector<string> splits_withBlanks = CommonTool::SplitString(line, ' ');
			//Erase null strings
			vector<string> splits;
			for (size_t i=0;i<splits_withBlanks.size();++i)
			{
				if (splits_withBlanks[i] != "")
				{
					splits.push_back(splits_withBlanks[i]);
				}
			}

			vector<PartOfSpeech> pattern_enum;
			for (size_t i = 0; i < splits.size() - 1; ++i)
			{
				pattern_enum.push_back((PartOfSpeech)atoi(splits[i].c_str()));
			}
			int frequency = atoi(splits.back().c_str());

			GrammarPattern pattern(pattern_enum);
			pattern.SetID(id++);

			if (pattern.Size() <= 1)
			{
				//the pattern is too small that contains less information.
				continue;
			}

			GrammarAttribute ga;
			ga.pattern=pattern;
			ga.frequency=frequency;
			patterns.push_back(ga);
		}

		return patterns;
	}
	
	

	std::vector<GrammarPattern> GrammarSet::ContainSubsequence( const GrammarPattern& pattern ) const
	{
		class ExtractSubsequences
		{
			vector<GrammarPattern> _sub;
			GrammarPattern _input;
		public:
			ExtractSubsequences(const GrammarPattern& pattern) :_input(pattern) {  }
			~ExtractSubsequences(){}

			vector<GrammarPattern> GetResult()const {return _sub;}
			void operator()(const GrammarAttribute& pattern)
			{
				if(_input.ContainSubsequence(pattern.pattern))
				{
					_sub.push_back(pattern.pattern);
				}
			}
		};

		ExtractSubsequences extractsub(pattern);
		extractsub=for_each(_patterns.begin(),_patterns.end(),extractsub);
		return extractsub.GetResult();
	}


	std::vector<DataCollection::GrammarPattern> GrammarSet::ContainedSubsequence( const DataCollection::GrammarPattern& pattern ) const
	{
		class ExtractParentsequences
		{
			vector<GrammarPattern> _parent;
			GrammarPattern _input;
		public:
			ExtractParentsequences(const GrammarPattern& pattern):_input(pattern){}
			~ExtractParentsequences(){}

			vector<GrammarPattern> GetResult()const {return _parent;}
			void operator()(const GrammarAttribute& pattern)
			{
				if(_input.IsSubsequenceOf(pattern.pattern))
				{
					_parent.push_back(pattern.pattern);
				}
			}
		};

		ExtractParentsequences extractParent(pattern);
		extractParent=for_each(_patterns.begin(),_patterns.end(),extractParent);
		return extractParent.GetResult();
	}


	int GrammarSet::CountOfSubsequence( const DataCollection::GrammarPattern& pattern ) const
	{
		class countSubsequences
		{
			GrammarPattern _input;
			int _count;
		public:
			countSubsequences(const GrammarPattern& pattern):_input(pattern),_count(0){}
			~countSubsequences(){}

			int GetResult()const {return _count;}
			void operator()(const GrammarAttribute& pattern)
			{
				if(_input.ContainSubsequence(pattern.pattern))
				{
					++_count;
				}
			}
		};

		countSubsequences countsub(pattern);
		countsub=for_each(_patterns.begin(),_patterns.end(),countsub);
		return countsub.GetResult();
	}

	int GrammarSet::GetFreqencyofPattern( const DataCollection::GrammarPattern& pattern ) const
	{
		if(pattern.Size()==0) return -1;
		if(pattern.GetID()<0) return -1;

		int id=pattern.GetID();
		if(id<0)
		{
			throw runtime_error("Error in GetFreqencyofPattern");
		}

		return _patterns[id].frequency;
	}

	void GrammarSet::AddGrammarPattern( DataCollection::GrammarPattern& pattern )
	{
		//ID为最大ID+1.
		pattern.SetID(GetMaxID()+1);
		GrammarAttribute g_a;
		g_a.pattern=pattern;
		g_a.frequency=1;

		_patterns.push_back(g_a);
	}

	void GrammarSet::IncreasePatternFreqency( const DataCollection::GrammarPattern& pattern )
	{
		int index=FindPatternIndex(pattern);
		if(index==-1)
		{
			GrammarPattern copy=pattern;
			AddGrammarPattern(copy);
		}
		else
		{
			GrammarAttribute g_a=_patterns[index];
			g_a.frequency++;
			_patterns[index]=g_a;
		}
		
	}

	void GrammarSet::AddPatternToTree( const DataCollection::GrammarPattern& pattern )
	{
		for (size_t j=-1;j<pattern.Size();++j)
		{
			pair<GrammarPattern,GrammarPattern> pattern_pair=pattern.Divide(j);
			GrammarPattern former=pattern_pair.first;
			if(former.Size())//former one is backward!!
			{
				_backwardtree[former.Get_ithElem(0)].AppendGramma(former);
			}
			GrammarPattern latter=pattern_pair.second;
			if(latter.Size())//latter one is forward!!
			{
				_forwardtree[latter.Get_ithElem(0)].AppendGramma(latter);
			}
		}
	}

	int GrammarSet::GetMaxID() const
	{
		class FindMaxID
		{
			int _max;
		public:
			FindMaxID():_max(0){}
			~FindMaxID(){}

			void operator()(const GrammarAttribute& g_a)
			{
				if(g_a.pattern.GetID()>_max)
				{
					_max=g_a.pattern.GetID();
				}
			}

			int GetResult(){return _max;}
		};

		FindMaxID findMaxID;
		findMaxID=for_each(_patterns.begin(),_patterns.end(),findMaxID);

		return findMaxID.GetResult();
	}

	int GrammarSet::FindPatternIndex( const DataCollection::GrammarPattern& pattern )
	{
		class SamePattern
		{
			GrammarPattern _pattern;

		public:
			SamePattern(const GrammarPattern& val):_pattern(val){}
			~SamePattern(){}

			bool operator()(const GrammarAttribute& g_a)
			{
				if(g_a.pattern.IsSameWith(_pattern))
				{
					return true;
				}
				else
				{
					return false;
				}
			}
		};
		vector<GrammarAttribute>::iterator find_iter=find_if(_patterns.begin(),_patterns.end(),SamePattern(pattern));
		if(find_iter==_patterns.end())
		{
			return -1;
		}
		else
		{
			return distance(_patterns.begin(),find_iter);
		}
	}

	Math::MyInt GrammarSet::GetTotalFrequency() const
	{
		Math::MyInt res(0);
		for (vector<GrammarAttribute>::const_iterator it=_patterns.begin();it!=_patterns.end();++it)
		{
			res += it->frequency;
		}

		return res;
	}

	vector<GrammarPattern> GrammarSet::GrammarPatternSortByFrequency() const
	{
		class SortAttribute
		{
		public:
			SortAttribute(){}
			~SortAttribute(){}

			bool operator()(const GrammarAttribute& left,const GrammarAttribute& right)
			{
				if(left.frequency>right.frequency)
				{
					return true;
				}
				else
				{
					return false;
				}
			}
		};

		//对频率从大到小排序
		vector<GrammarAttribute> grammarAttributes=_patterns;
		sort(grammarAttributes.begin(),grammarAttributes.end(),SortAttribute());

		class ExtracePattern
		{
		public:
			ExtracePattern(){}
			~ExtracePattern(){}

			GrammarPattern operator()(const GrammarAttribute& g_a)
			{
				return g_a.pattern;
			}
		};

		//把GrammarAttribute转换成GrammarPattern
		vector<GrammarPattern> res;
		transform(grammarAttributes.begin(),grammarAttributes.end(),back_inserter(res),ExtracePattern());

		return res;
	}

// 	double GrammarSet::ComputePossibility(const DataCollection::GrammarPattern& pattern) const
// 	{
// 		//The final possibility takes local grammar and grammar patterns of <pattern> into consideration.
// 		//And the above two components has weights respectively.
// 		MyInt totalFreq = GetTotalFrequency();
// 		double localP = ComputeP_GrammarLocalAnalysis(pattern);
// 		double patternP = _patternModel->ComputePossiblity(pattern, totalFreq);
// 		//DEBUG_FORMAT2("Local grammar possibility is %.10lf.Grammar pattern possibility is %.10lf.", localP, patternP);
// 		double res = _wPattern*patternP + _wLocal*localP;
// 		
// 		//Adjust the possibility such that it is in the interval of 0 to 1.
// 		res = min(1., res);
// 		res = max(0., res);
// 
// 		return res;
// 	}

	map<double, PartOfSpeech> GrammarSet::ComputePossibilityTable(const DataCollection::PartOfSpeech& forwardPos, const DataCollection::PartOfSpeech& backwardPos) const
	{
		map<double, PartOfSpeech> res;

		for (int i=0;i<NUM_PARTOFSPEECH;++i)
		{
			double possi =_localModel->ComputeP_GrammarLocal((PartOfSpeech)i, forwardPos, backwardPos);
			res[possi]= (PartOfSpeech)i;
		}

		return res;
	}

	double GrammarSet::ComputeGrammarPossibility(const vector<shared_ptr<DataCollection::Word>>& sentence) const
	{
		if (!_featureModel->FeaturesLoaded())
		{
			_featureModel->LoadAllFeatures();
		}

		return _featureModel->ComputePossiblity(sentence);
	}

	double GrammarSet::ComputeP_GrammarLocalAnalysis(const vector<shared_ptr<DataCollection::Word>>& sentence) const
	{
		return _localModel->ComputePossiblity(sentence);
	}

	void GrammarSet::InitializeWeights()
	{
// 		vector<Sen_Gra> samples = CommonFunction::InputGraSamples(GetHopeLoveMindPath() + StringGrammar_InitialFilename);
// 
// 		//Compute possibilities of grammar patterns and local grammar of each pattern.
// 		MyInt totalFreq = GetTotalFrequency();
// 		vector<double> localPVec;
// 		vector<double> patternPVec;
// 		for (size_t i = 0; i < samples.size(); ++i)
// 		{
// 			//Compute the possibility of local grammar.
// 			GrammarPattern pattern=LanguageFunc::ConvertToPattern(samples[i].gra);
// 			double localP = ComputeP_GrammarLocalAnalysis(pattern);
// 			localPVec.push_back(localP);
// 
// 			//Compute the possibility of matched patterns.
// 			double patternP = _patternModel->ComputePossiblity(pattern, totalFreq);
// 			patternPVec.push_back(patternP);
// 		}
// 
// 		//Optimize <_wPattern> and <_wLocal> until possibility of each sample is close to 1.
// 		_wPattern = 0.5, _wLocal = 0.5;
// 		ComputeWeights(patternPVec, localPVec, _wPattern, _wLocal);
// 
// 		WriteWeights(_wPattern, _wLocal);
	}

	void GrammarSet::WriteWeights(const double wPattern, const double wLocal)
	{
		string paramFile = GetHopeLoveMindPath() + "MindParam.txt";
		TiXmlDocument *myDocument = new TiXmlDocument(paramFile.c_str());
		myDocument->LoadFile();
		TiXmlNode *root = myDocument->FirstChild("Root");

		TiXmlNode *doubleParamNode = root->FirstChild("DoubleParam");
		TiXmlElement *wPatternNode = doubleParamNode->FirstChildElement("wPattern");
		TiXmlElement *wLocalNode = doubleParamNode->FirstChildElement("wLocal");

		if (wPatternNode == nullptr || wLocalNode == nullptr)
		{
			throw runtime_error("Error in ReadWeights: cannot write weights to file.");
		}

		wPatternNode->SetAttribute("value", to_string(wPattern).c_str() );
		wLocalNode->SetAttribute("value", to_string(wLocal).c_str());

		delete myDocument;
	}

	void GrammarSet::ReadWeights()
	{
		string paramFile = GetHopeLoveMindPath()+"MindParam.txt";
		TiXmlDocument *myDocument = new TiXmlDocument(paramFile.c_str());
		if (!myDocument->LoadFile())
		{
			cout << "Cannot read weights in GrammarSet" << endl;
			return;
		}
		TiXmlNode *root = myDocument->FirstChild("Root");

		TiXmlNode *doubleParamNode=root->FirstChild("DoubleParam");
		TiXmlElement *wPatternNode=doubleParamNode->FirstChildElement("wPattern");
		TiXmlElement *wLocalNode = doubleParamNode->FirstChildElement("wLocal");

		if (wPatternNode == nullptr || wLocalNode == nullptr)
		{
			throw runtime_error("Error in ReadWeights: cannot read weights from file.");
		}

		const char* wpatternStr=wPatternNode->Attribute("value");
		const char* wlocalStr = wLocalNode->Attribute("value");

		_wPattern= strtod(wpatternStr, NULL);
		_wLocal = strtod(wlocalStr, NULL);

		delete myDocument;
	}

	void GrammarSet::ComputeWeights(const vector<double>& patternP, const vector<double>& localP,
		double& wPattern, double& wLocal) const
	{
		double gradPattern, gradLocal;
		double deltaStep = 1e-4;
		double divergeTol = 1e-5;
		int iterCount = 0;
		int maxIterCount = 10000;
		do 
		{
			double dev = ComputeDeviation(patternP, localP, wPattern, wLocal);
			//Numerically compute the gradient of each weight.
			double deltaPattern= ComputeDeviation(patternP, localP, wPattern+ deltaStep, wLocal)-dev;
			double deltaLocal = ComputeDeviation(patternP, localP, wPattern , wLocal + deltaStep) - dev;
			gradPattern = deltaPattern / deltaStep;
			gradLocal = deltaLocal / deltaStep;

			//Adjust each weight simply by gradient.
			wPattern -= gradPattern;
			wLocal -= gradLocal;

			if (++iterCount > maxIterCount)
			{
				break;
			};

			//Iterate until both weights are not changed within tolerance.
		} while (Math::DoubleCompare(gradPattern,0,divergeTol)!=0 && Math::DoubleCompare(gradLocal, 0, divergeTol) != 0);

	}

	double GrammarSet::ComputeDeviation(const vector<double>& patternP, const vector<double>& localP,
		const double wPattern, const double wLocal) const
	{
		assert(patternP.size() == localP.size());

		double res = 0;
		for (size_t i=0;i<patternP.size();++i)
		{
			//Each element of <patternP> and the element in <localP> of the same index correspond to one grammar pattern.
			//Sum possibilities of each grammar patterns and compute the deviation from one.
			res += pow(patternP[i] * wPattern + localP[i] * wLocal-1,2);
		}

		//Normalize deviation.
		return res/patternP.size();
	}


	double GrammarSet::GetP_Forward(const DataCollection::PartOfSpeech& me, const DataCollection::PartOfSpeech& forward) const
	{
		return _localModel->GetP_Forward(me, forward);
	}

	double GrammarSet::GetP_Backward(const DataCollection::PartOfSpeech& me, const DataCollection::PartOfSpeech& backward) const
	{
		return _localModel->GetP_Backward(me, backward);
	}

	double GrammarSet::ComputePossibility(const vector<shared_ptr<DataCollection::Word>>& sentence) const
	{
		//The final possibility takes local grammar and grammar patterns of <pattern> into consideration.
		//And the above two components has weights respectively.

		double localP = _localModel->ComputePossiblity(sentence);
		double patternP = _patternModel->ComputePossiblity(sentence);
		//DEBUG_FORMAT2("Local grammar possibility is %.10lf.Grammar pattern possibility is %.10lf.", localP, patternP);
		double res = _wPattern*patternP + _wLocal*localP;

		//Adjust the possibility such that it is in the interval of 0 to 1.
		res = min(1., res);
		res = max(0., res);

		return res;
	}

}

