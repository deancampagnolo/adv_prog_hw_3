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
         return;
      }
   }
   if (argument.find("=") == argument.length() -1) {
      cout << "delete key and value pair that matches key"<<endl;
      return;
   } else {
      cout << "insert/replace key value pair"<<endl;
      str_str_pair the_pair("a", "bj");
      the_map->insert(the_pair);
      return;
   }
}

int main (int argc, char** argv) {
   fstream the_file;

   str_str_map test;


   if (argc > 1) {
      for (int item = 1; item <argc; item ++) {
         the_file.open(argv[item]);
         if (the_file.is_open()) {
            string current_line;
            while(getline(the_file, current_line)){
               cout <<current_line<<endl;
               do_command(current_line, &test);
            }
            the_file.close();
         }

      }
   }

   return -1;

   sys_info::execname (argv[0]);
   scan_options (argc, argv);

   //str_str_map test;
   for (char** argp = &argv[optind]; argp != &argv[argc]; ++argp) {
      str_str_pair pair (*argp, to_string<int> (argp - argv));
      cout << "Before insert: " << pair << endl;
      //test.insert (pair);
   }

   cout << test.empty() << endl;
   for (str_str_map::iterator itor = test.begin();
        itor != test.end(); ++itor) {
      cout << "During iteration: " << *itor << endl;
   }

   str_str_map::iterator itor = test.begin();
   test.erase (itor);

   cout << "EXIT_SUCCESS" << endl;
   return EXIT_SUCCESS;
}

