﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using GlobalInterface;

namespace DataWrapperCS
{
    public class DataWrapper_CS : GlobalInterface.DataWrapper
    {
        public string Command { get; }

        string _inputsentence;
        string _outputsentence;

        public DataWrapper_CS(string command = "chatting")
        {
            this.Command = command;
        }

        public object GetData(DataType type)
        {
            switch (type)
            {
                case DataType.InputString:
                    {
                        return _inputsentence;
                    }
                case DataType.OutputString:
                    {
                        return _outputsentence;
                    }
            }

            return null;
        }

        public void SetData(DataType type, object data)
        {
            switch (type)
            {
                case DataType.InputString:
                    {
                        string str = data as string;
                        if (str != null)
                        {
                            _inputsentence = str;
                        }
                        break;
                    }
                case DataType.OutputString:
                    {
                        string str = data as string;
                        if (str != null)
                        {
                            _outputsentence = str;
                        }
                        break;
                    }
            }
        }

        public string GetCommandName()
        {
            return this.Command;
        }
    }
}
