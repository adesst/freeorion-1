// -*- C++ -*-
#ifndef _DataTable_h_
#define _DataTable_h_

#include <map>
#include <string>
#include <vector>

#include "Export.h"

/** the type of a text configuration file data table */
typedef std::vector<std::vector<int> > DataTable;

/** the type of a text configuration file data table.  Usually, one of these is defined per config file; 
    each entry is keyed on the name of the table (e.g. "DensityModToPlanetSizeDist"). */
typedef std::map<std::string, DataTable> DataTableMap;

/** loads all the tables from file \a filename, placing them in \a tables.  The config file can be echoed 
    to stdout by setting echo to true; this is useful in debugging a incorrect config file. */
FO_COMMON_API void LoadDataTables(const std::string& filename, DataTableMap& tables, bool echo = false);

#endif // _DataTable_h_
