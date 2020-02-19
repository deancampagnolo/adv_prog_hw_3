// $Id: main.cpp,v 1.11 2018-01-25 14:19:29-08 - - $

#include <cstdlib>
#include <exception>
#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>

using namespace std;

#include "listmap.h"
#include "xpair.h"
#include "util.h"

using str_str_map = listmap<string,string>;
using str_str_pair = str_str_map::value_type;

void scan_options (int argc, char** argv) {
   opterr = 0;
   for (;;) {
      int option = getopt (argc, argv, "@:");
      if (option == EOF) break;
      switch (option) {
         case '@':
            debugflags::setflags (optarg);
            break;
         default:
            complain() << "-" << char (optopt) << ": invalid option"
                       << endl;
            break;
      }
   }
}


void do_command(string argument, str_str_map* the_map) {
   cout << "\t";
   if (argument.find("=") == std::string::npos) {
      cout << "print key and value pair" << endl;
      return;
   }
   if (argument.at(0) == '=') {
      if (argument.length() == 1) {
         cout << "print all key and value pairs lexographically"<<endl;
         return;
      }else {
         cout << "print key and value pairs with given value"<<endl;
         cout << argument.substr(1)<<endl;
         return;
      }
   }
   if (argument.find("=") == argument.length() -1) {
      cout << "delete key and value pair that matches key"<<endl;
      cout << argument.substr(0,argument.length()-1);
      return;
   } else {
      str_str_pair the_pair(argument.substr(0,argument.find("=")),
         argument.substr(argument.find("=")+1));
      cout << "insert/replace key value pair"<<the_pair<<endl;
      the_map->insert(the_pair);
      return;
   }
}

int main (int argc, char** argv) {
   fstream the_file;

   str_str_map test;

   string current_line;
   cerr<<"ahhh"<<endl;
   //complain()<<"oh no!"<<endl;
   if (argc > 1) {
      for (int item = 1; item <argc; item ++) {
         the_file.open(argv[item]);
         if (the_file.is_open()) {
            int count = 1;
            while(getline(the_file, current_line)){
               cout << argv[item] <<": " << count<<": " <<current_line
                  <<endl;
               do_command(current_line, &test);
               count++;
            }
            the_file.close();
         }

      }
   } else {
      int count = 1;
      while(getline(cin, current_line)){
         cout << count<<": "<<current_line << endl;
         do_command(current_line, &test);
         //cout << "hmm"<<endl;
      }
   }

   cout <<"is it even possible to get here"<<endl;

   sys_info::execname (argv[0]);
   scan_options (argc, argv);

   cout << test.empty() << endl;
   for (str_str_map::iterator itor = test.begin();
        itor != test.end(); ++itor) {
      cout << "During iteration: " << *itor << endl;
   }

   str_str_map::iterator itor = test.begin();
   test.erase (itor);

   cout << "EXIT_SUCCESS" << endl;

   sys_info::exit_status_;
   return sys_info::exit_status_;
}

