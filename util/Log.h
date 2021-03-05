#ifndef _LOG_H
#define _LOG_H

#include <string>
#include <fstream>
#include <vector>

class LogEntry
{
    public:
    int term;
    int index;
    std::string value;
    public:
    LogEntry(int t, int i, std::string v): term(t), index(i), value(v){}
    LogEntry(int t, std::string v): term(t), value(v){}
    LogEntry(){}
};

class Log
{
    public:
    // path to write log to disk
    std::string path;
    std::ofstream lFile;
    std::vector<LogEntry> entries;
    uint64_t commitIndex;
    void (*applyFunc)(void *);

    public:

    // lenth: get log length
    // parameters: void
    // outputs   : log's lenth
    int lenth()
    {
        return entries.size();
    }

    // getLastLogIndex: get last log entry's index
    // parameters: void
    // outputs   : last log entry's index
    int getLastLogIndex()
    {
        return entries.size() - 1;
    }

    // getPrevLogIndex: get the index of prev log entry
    // parameters: index-prev entry's index
    // outputs   : prev log index
    int getPrevLogIndex(int index)
    {
        return index - 1;
    }

    // getLastLogTerm: get the term of last log entry
    // paramters: void
    // outputs  : term of last log entry
    int getLastLogTerm()
    {
        if(entries.size() >= 1)
        {
            return entries[entries.size() - 1].term;
        }
        else
        {
            return 0;
        }
    }

    // getPrevLogTerm: get the term of prev log entry
    // paramters: void
    // outputs  : term of prev log entry
    int getPrevLogTerm(int index)
    {
        if(index > 0)
        {
            return entries[index - 1].term;
        }
        else
        {
            return 0;
        }
        
    } 

    // check: check if there exit entry at prevLogIndex 
    //        which term equals to prevLogTerm
    // parameters: prevLogIndex-index of entry prior to which send next
    //             prevLogTerm-term of entry prior to which send next
    // outputs   : true-if term of entry at prevLogIndex equals to prevLogTerm
    //             false-otherwise
    bool check(int prevLogIndex, int prevLogTerm)
    {
        if(prevLogIndex == -1)
        {
            return true;
        }

        if(entries.size() <= prevLogIndex)
        {
            return false;
        }
        else
        {
            if(entries[prevLogIndex].term != prevLogTerm)
            {
                return false;
            }
        }
        return true;
    }

    // del: delete entries which start at startIndex
    // parameters: startIndex-the start delete index
    // outputs   : void
    void del(int startIndex)
    {
        if(startIndex < entries.size())
        {
            entries.erase(entries.begin() + startIndex, entries.end());
        }
    }
};

#endif // _LOG_H
