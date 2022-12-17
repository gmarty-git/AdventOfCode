#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <utility>
#include <numeric>   

using namespace std;

class Packet {
 public:

    Packet() {
        isInt = false;
    }

    Packet(int intElem) {
        this->intElem = intElem;
        isInt = true;
    }
    
    Packet(string strPacket) {
        isInt = false;
        stringstream str_strm(strPacket);
        str_strm.ignore(1, '[');
        constructPacketFromStream(str_strm, *this); 
    }
    
    void constructPacketFromStream(istream &is, Packet &parentPacket) {
        char c;
        int i;       
        
       do {
            c = is.get();

            if (c == ']'){
                return;
            }
            else if (c == '['){
                parentPacket.packetsVector.emplace_back();
                constructPacketFromStream(is, parentPacket.packetsVector.back());  
                if (is.peek() != is.eof())
                    c = is.get();
            }
            else {
                is.unget();
                is >> i;
                parentPacket.packetsVector.emplace_back(i);
                c = is.get();
            }
        }  while ( c != ']' );
    }
    
    friend bool operator== (Packet &left, Packet &right) {
        if ( left.isInt && right.isInt )
            return left.intElem == right.intElem ;
     
       else if (  !left.isInt && !right.isInt ){
            if (left.packetsVector.size() != right.packetsVector.size())
                return false;

            for (int i = 0 ; i < left.packetsVector.size() ; ++i){
                if (!(left.packetsVector[i] == right.packetsVector[i]))
                    return false;
            }

            return true;
        }
        else if (left.isInt) {
            if (right.packetsVector.size() != 1)
                return false;
            
            return left == right.packetsVector[0];
        }
        else if (right.isInt) {
            if (left.packetsVector.size() != 1)
                return false;
            
            return right == left.packetsVector[0];
        }
    }   

    friend bool operator< (Packet &left, Packet &right){
        if ( left.isInt && right.isInt )
            return left.intElem < right.intElem ;

        else if (  !left.isInt && !right.isInt ){
            for (int i = 0 ; i < min(left.packetsVector.size() , right.packetsVector.size()) ; ++i){
                if (left.packetsVector[i] == right.packetsVector[i])
                    continue;

                else 
                    return left.packetsVector[i] < right.packetsVector[i] ;          
            }

            return left.packetsVector.size() < right.packetsVector.size() ;
        }
        else if (left.isInt) {
            string str_Packet = "[" + to_string(left.intElem) + "]";
            Packet pckt_List(str_Packet);
            return  pckt_List < right ;
        }
        else {
            string str_Packet = "[" + to_string(right.intElem) + "]";
            Packet pckt_List(str_Packet);
            return left < pckt_List;
        }
    }   

    friend ostream& operator<< (ostream &os, Packet &packet) {
        if (packet.isInt)
                os << packet.intElem;
        else {
            os << "[" ;
            for (auto it = packet.packetsVector.begin() ; it != packet.packetsVector.end() ; ++it){
                os << *it;
                if (it != packet.packetsVector.end() - 1)
                  os << ", ";
            }
              os << ']';
        }
      
        return os;
    }        

 private:
    int intElem;
    vector<Packet> packetsVector;
    bool isInt;

};


int main (int argc, char **argv) {
   ifstream ifs_input ("input_13.txt");
   string str_p1, str_p2;
    
   using pcktPair = pair<Packet, Packet>;  
   vector<pcktPair> pcktPairs;
   while (getline(ifs_input, str_p1)) {
        getline(ifs_input, str_p2);
        Packet p1(str_p1), p2(str_p2);
        pcktPairs.push_back(pcktPair {Packet(str_p1), Packet(str_p2)}); 
        getline(ifs_input, str_p1);
    }

   //Part1
   //---------------------------------------------------------
   int idxSum=0;
   for (int i = 0 ; i < pcktPairs.size() ; ++i)
        if (pcktPairs[i].first < pcktPairs[i].second)
            idxSum += i+1;
   //---------------------------------------------------------

   //Part2
   //---------------------------------------------------------
   //flaten packets pairs
   vector<Packet> allPkt;
   for (auto it = pcktPairs.begin() ; it != pcktPairs.end() ; ++it) {
        allPkt.push_back(it->first);
        allPkt.push_back(it->second);
   }

   //insert divider packets
   allPkt.push_back(Packet("[[2]]"));
   allPkt.push_back(Packet("[[6]]"));

   //sort packets
   vector<unsigned int> permutation(allPkt.size());
   iota(permutation.begin(), permutation.end(), 0);
   sort(permutation.begin(), permutation.end(), [&allPkt](unsigned int a, unsigned int b) { return allPkt[a] < allPkt[b];});

   //extract decoder key
   int decoderKey = 1;
   for (int i = 0 ; i < permutation.size() ; ++i) {
       if ( ( permutation[i] == permutation.size() - 1 ) || ( permutation[i] == permutation.size() - 2) )
           decoderKey *= (i+1);
    }
    
    //display sorted packets
    for (int i = 0 ; i < allPkt.size() ; ++i) {
        cout << allPkt[permutation[i]] << endl;
    }
   //---------------------------------------------------------
 
   cout << endl;
   cout << "Day 13 - Part1  anwswer : " <<  idxSum << endl;
   cout << "Day 13 - Part2  anwswer : " <<  decoderKey << endl;
    
   exit(0);
}
