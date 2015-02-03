#include <iostream>
#include <sstream>
#include <stdexcept>
//#include <vector>
//#include <algorithm>
//#include <iterator>
#include "boost/foreach.hpp"
// #include "boost/preprocessor.hpp"
#include "boost/algorithm/string/trim.hpp"
#include "boost/algorithm/string/erase.hpp"

#include "alps/params.hpp"

/* Supported parameter types: */
#ifndef ALPS_PARAMS_SUPPORTED_TYPES
#define ALPS_PARAMS_SUPPORTED_TYPES (5, (int,unsigned,double,bool,std::string))
#endif


namespace alps {
    namespace params_ns {
  
        namespace po=boost::program_options;
    
        const char* const params::cfgfile_optname_="parameter-file";

        params::params(hdf5::archive ar, std::string const & path)
        {
            throw std::logic_error("Not implemented yet");
        }

        /** Access a parameter: read-only */
        const params::mapped_type& params::operator[](const std::string& k) const
        {
            possibly_parse();
            return optmap_[k];
        }

        /** Access a parameter: possibly for assignment */
        params::mapped_type& params::operator[](const std::string& k)
        {
            possibly_parse();
            return optmap_[k];
        }


        /// Function to check for redefinition of an already-defined option (throws!)
        void params::check_redefine(const std::string& optname) const
        {
            if (anycast_map_.count(optname)) {
                // The option was already defined
                throw std::runtime_error("Option redefenition"); // FIXME: include name in the message
            }
            if (optmap_.count(optname)) {
                // The option was already explicitly assigned or defined
                throw std::runtime_error("Attempt to define assigned option"); // FIXME: include name in the message
            }
        }
        
        
        void params::certainly_parse() const
        {
            po::positional_options_description pd;
            pd.add(cfgfile_optname_,1); // FIXME: should it be "-1"? How the logic behaves for several positional options?
            po::parsed_options cmdline_opts=
                po::command_line_parser(argvec_).
                allow_unregistered().
                options(descr_).
                positional(pd).  
                run();

            po::variables_map vm;
            po::store(cmdline_opts,vm);

            if (vm.count(cfgfile_optname_) != 0) {
                const std::string& cfgname=vm[cfgfile_optname_].as<std::string>();
                po::parsed_options cfgfile_opts=
                    po::parse_config_file<char>(cfgname.c_str(),descr_,true); // parse the file, allow unregistered options

                po::store(cfgfile_opts,vm);
            }

            // Now copy the parsed options to the this's option map
            // NOTE: if file has changed since the last parsing, option values will be reassigned!
            BOOST_FOREACH(const po::variables_map::value_type& slot, vm) {
                const std::string& k=slot.first;
                const boost::any& val=slot.second.value();
                anycast_map_type::const_iterator assign_it=anycast_map_.find(k);
                assert(assign_it!=anycast_map_.end()
                       && "Key always exists in anycast_map_: only explicitly-defined options are processed here");
                const assign_fn_type& assign_fn=assign_it->second;
                (optmap_[k].*assign_fn)(val); // using the saved assign_any() function to assign the value
            }
        }        

        void params::save(hdf5::archive& ar) const
        {
            throw std::logic_error("params::save() is not implemented yet");
        }

        void params::load(hdf5::archive& ar)
        {
            throw std::logic_error("params::load() is not implemented yet");
        }

        bool params::help_requested(std::ostream& ostrm)
        {
            possibly_parse();
            if (optmap_.count("help")) { // FIXME: will conflict with explicitly-assigned "help" parameter
                ostrm << helpmsg_ << std::endl;
                ostrm << descr_;
                return true;
            }
            return false;
        }        

        
        // /// Output parameters to a stream
        // std::ostream& operator<< (std::ostream& os, const params& prm)
        // {
        //     BOOST_FOREACH(const params::value_type& pair, prm) {
        //         const std::string& k=pair.first;
        //         const params::mapped_type& v=pair.second;
        //         os << k << "=";
        //         // FIXME: the following game with iterators and assertions can be avoided
        //         //        if the printout function would be a member of params::mapped_type;
        //         //        however, it requires deriving from boost::variables_map.
        //         params::printout_map_type::const_iterator pit=prm.printout_map_.find(k);
        //         assert(pit != prm.printout_map_.end() && "Printout function is given for a parameter");
        //         (pit->second)(os,v.value());
        //         os << std::endl;
        //     }
        //     return os;
        // }

    } // params_ns
} // alps

namespace boost {
    namespace program_options {
        // Declaring validate() function in the boost::program_options namespace for it to be found by boost.
        void validate(boost::any& outval, const std::vector<std::string>& strvalues,
                      std::string* target_type, int)
        {
            namespace po=boost::program_options;
            namespace pov=po::validators;
            namespace alg=boost::algorithm;
            typedef std::vector<std::string> strvec;
            typedef boost::char_separator<char> charsep;
        
            pov::check_first_occurrence(outval); // check that this option has not yet been assigned
            std::string in_str=pov::get_single_string(strvalues); // check that this option is passed a single value

            // Now, do parsing:
            alg::trim(in_str); // Strip trailing and leading blanks
            if (in_str[0]=='"' && in_str[in_str.size()-1]=='"') { // Check if it is a "quoted string"
                // Strip surrounding quotes:
                alg::erase_tail(in_str,1);
                alg::erase_head(in_str,1);
            }
            outval=boost::any(in_str);
            // std::cerr << "***DEBUG: returning from validate(...std::string*...) ***" << std::endl;
        }
    } // program_options
} // boost
