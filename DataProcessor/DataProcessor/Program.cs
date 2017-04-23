using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;

namespace DataProcessor
{
    public enum PartOfSpeech
    {
        Noun,
        Verb,
        Adjective,
        Numeral,
        Quantifier,
        Pronoun,
        Adverb,
        Preposition,
        Conjunction,
        Auxiliary,
        Onomatopoeia,
        Interjection,
        Ambiguous,//Ambiguous indicates a word is known by AI but has no definite POS temporarily.
        Unknown,//Unknown indicates a word is unknown.
        Punctuation,
        Other
    };

    class Program
    {
        static void Main(string[] args)
        {
            ProcessPOSTagged processor = new ProcessPOSTagged();
            processor.Do(@"C:\Users\Administrator\Desktop\myconv.txt");

            //             POSAssignment posAssign = new POSAssignment();
            //             posAssign.SetMaxIDFOfCommonWords(13);
            //             posAssign.AssignPOSofCommonWords();
            //            posAssign.GenerateLargeVocabulary();

            //                         SegmentationCorpusExtractor sce = new SegmentationCorpusExtractor();
            //                         sce.Run("wo","我");

            //             ReduceGrammarPatternDistribution rgpd = new ReduceGrammarPatternDistribution();
            //             rgpd.Run(Dir.outdir + "GrammaPatterns_Initial_corpus_de.txt", Dir.outdir + "GrammaPatterns_Initial_corpus_de_reduced.txt");

            //             ExtractChatting extractChatting = new ExtractChatting();
            //             extractChatting.Extract(@"C:\Users\Administrator\Desktop\莫莫.txt");
        }
    }
}
