using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;

namespace DataProcessor
{
    class POSMapping
    {
        static Dictionary<string, PartOfSpeech> posTable;

        static POSMapping()
        {
            posTable = new Dictionary<string, PartOfSpeech>();
            posTable.Add("n", PartOfSpeech.Noun);
            posTable.Add("nr", PartOfSpeech.Noun);
            posTable.Add("ns", PartOfSpeech.Noun);
            posTable.Add("nd", PartOfSpeech.Noun);
            posTable.Add("nt", PartOfSpeech.Noun);
            posTable.Add("nl", PartOfSpeech.Noun);
            posTable.Add("nh", PartOfSpeech.Noun);
            posTable.Add("nhf", PartOfSpeech.Noun);
            posTable.Add("nhg", PartOfSpeech.Noun);
            posTable.Add("ni", PartOfSpeech.Noun);
            posTable.Add("nz", PartOfSpeech.Noun);
            posTable.Add("v", PartOfSpeech.Verb);
            posTable.Add("vd", PartOfSpeech.Verb);
            posTable.Add("vl", PartOfSpeech.Verb);
            posTable.Add("vu", PartOfSpeech.Verb);
            posTable.Add("a", PartOfSpeech.Adjective);
            posTable.Add("m", PartOfSpeech.Numeral);
            posTable.Add("q", PartOfSpeech.Quantifier);
            posTable.Add("r", PartOfSpeech.Pronoun);
            posTable.Add("d", PartOfSpeech.Adverb);
            posTable.Add("t", PartOfSpeech.Adverb);
            posTable.Add("p", PartOfSpeech.Preposition);
            posTable.Add("c", PartOfSpeech.Conjunction);
            posTable.Add("u", PartOfSpeech.Auxiliary);
            posTable.Add("o", PartOfSpeech.Onomatopoeia);
            posTable.Add("e", PartOfSpeech.Interjection);
            posTable.Add("w", PartOfSpeech.Punctuation);
        }

        public static PartOfSpeech GetMyPOS(string str)
        {
            if (posTable.ContainsKey(str))
            {
                return posTable[str];
            }
            else
                throw new ArgumentOutOfRangeException();
        }

    }

    /// <summary>
    /// Used for process text of POS tagging by CorpusWordParser.exe.
    /// </summary>
    class ProcessPOSTagged
    {
        /// <summary>
        /// Firstly use CorpusWordParser.exe to POS-tag some text , then save it to DataProcessor\Corpus\Raw,and call this method.
        /// </summary>
        public void Do(string filename)
        {
            StreamReader sr = new StreamReader(filename,Encoding.UTF8);

            StreamWriter sw = new StreamWriter(Path.Combine(Dir.newdir, Path.GetFileName(filename)));

            while (!sr.EndOfStream)
            {


                //Move the excess blank between the two words and mapping POS to my PartOfSpeech

                string line = sr.ReadLine();
                try
                {
                    List<string> wordlist = new List<string>();

                    var split = line.Split(' ');
                    foreach (var s in split)
                    {
                        if (s != "")
                            wordlist.Add(s);
                    }

                    string posLine = "";
                    string rawSentence = "";
                    for (int i = 0; i < wordlist.Count; ++i)
                    {
                        var word = wordlist[i];
                        var word_pos = word.Split('/');
                        if (word_pos.Length != 2) throw new Exception();

                        PartOfSpeech myPOS = POSMapping.GetMyPOS(word_pos[1]);
                        var newWord = String.Format("{0}/{1}", word_pos[0], Convert.ToInt32(myPOS));

                        posLine += newWord;
                        rawSentence += word_pos[0];
                        if (i != wordlist.Count - 1)
                        {
                            posLine += " ";
                        }
                    }

                    sw.WriteLine(rawSentence);
                    sw.WriteLine(posLine);
                }
                catch (System.Exception ex)
                {

                }

            }

            sw.Flush();
        }

    }
}
