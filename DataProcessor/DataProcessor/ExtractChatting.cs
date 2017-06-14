using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;

namespace DataProcessor
{
    /// <summary>
    /// A basic element in a chatting dialog,  including a sentence and the role saying this sentence and so on.
    /// </summary>
    class ChattingElement
    {
        public string Role { get; set; }
        public string Sentence { get; set; }
    }

    /// <summary>
    /// Chatting pair of Chatting element.
    /// It contains the chatting element and response of the element.
    /// </summary>
    class ChattingPair
    {
        public ChattingElement First { get; set; }
        public ChattingElement Second { get; set; }
    }

    /// <summary>
    /// Extract chatting dialog for QQ dialog.
    /// </summary>
    public partial class ExtractChatting
    {

        /// <summary>
        /// Roles in the chatting.
        /// </summary>
        const string _role1 = "莫莫、";
        const string _role2 = "费曼的彩虹";


        public ExtractChatting()
        {
            
        }

        public void Extract(string filename)
        {
            if (!File.Exists(filename)) return;

            List<ChattingElement> elements = ExtractElement(filename);

            //Filter undesired sentences.
            FilterChatting(elements);

            //Output sentences in conversation.
            OutputConversation(elements);

            //Extract elements and their responses.
            //List<ChattingPair> chattingPairs = ExtractChattingPairs(elements);
        }

        private void FilterChatting(List<ChattingElement> chattingElem)
        {
            for (int i = chattingElem.Count - 1; i >= 0; --i)
            {
                if (ShouldFiltered(chattingElem[i]))
                {
                    chattingElem.Remove(chattingElem[i]);
                }
            }
        }

        private bool ShouldFiltered(ChattingElement element)
        {
            //Picture and expression contains brackets like "[表情]", "[图片]".
            const char leftBra = '[';
            const char rightBra = ']';
            if (element.Sentence.Contains(leftBra) && element.Sentence.Contains(rightBra))
            {
                return true;
            }

            //Filter web site.
            if (element.Sentence.Contains("http") || element.Sentence.Contains("www"))
            {
                return true;
            }

            if(element.Sentence.Trim()=="")
            {
                return true;
            }

            return false;
        }

        private void OutputConversation(List<ChattingElement> chattingElem)
        {
            StreamWriter sw = new StreamWriter(Dir.outdir + "Conversation Sample.txt");
            foreach (ChattingElement elem in chattingElem)
            {
                sw.Write(elem.Sentence + "\r\n");
            }

            sw.Flush();
            sw.Close();
        }

        private void OutputChattingPairs(List<ChattingPair> chattingPairs)
        {
            StreamWriter sw = new StreamWriter(Dir.outdir + "Conversation Pairs Sample.txt");

            for (int i = 0; i < chattingPairs.Count; ++i)
            {
                sw.Write(chattingPairs[i].First.Sentence + "\r\n");
                sw.Write(chattingPairs[i].Second.Sentence);

                if (i != chattingPairs.Count - 1)
                {
                    sw.Write("\r\n");
                }
            }

            sw.Flush();
            sw.Close();
        }

        private List<ChattingElement> ExtractElement(string filename)
        {
            List<ChattingElement> res = new List<ChattingElement>();

            StreamReader sr = new StreamReader(filename);
            while (!sr.EndOfStream)
            {
                string line = sr.ReadLine();
                if (line == "") continue;

                string role = ExtractRole(line);
                if (role == "") continue;

                //Only if the current line has a role, the next line is the sentence the role says.
                ChattingElement element = new ChattingElement();
                element.Role = role;
                element.Sentence = sr.ReadLine();

                res.Add(element);
            }

            return res;
        }

        private string ExtractRole(string line)
        {
            //Check whether the first term is a date.
            //If not , it is not the start of the element.
            string[] split = line.Split(' ');
            DateTime time;
            if (!DateTime.TryParse(split[0], out time))
            {
                return "";
            }

            if (split.Length != 3)
            {
                return "";
            }

            if (split[2] == _role1)
            {
                return _role1;
            }
            else if (split[2] == _role2)
            {
                return _role2;
            }
            else
            {
                return "";
            }
        }


        private List<ChattingPair> ExtractChattingPairs(List<ChattingElement> elements)
        {
            List<ChattingPair> res = new List<ChattingPair>();

            for (int i = 0; i < elements.Count - 1; ++i)
            {
                //If the current role is not the role of next element, then append a chatting pair.
                if (elements[i].Role != elements[i + 1].Role)
                {
                    ChattingPair pair = new ChattingPair();
                    pair.First = elements[i];
                    pair.Second = elements[i + 1];

                    res.Add(pair);
                }
            }

            return res;
        }
    }
}
