    /*

    Copyright (C) 2000 Stefan Westerfeld
                       stefan@space.twc.de

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.
  
    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.
   
    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.

    */

#include "mcoputils.h"
#include "mcopconfig.h"
#include "debug.h"
#include <pwd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <netdb.h>
#include <errno.h>
#include <config.h>
#include <ctype.h>
#include <cstdlib>
#include <cstring>
#include <stdio.h>
#include <map>
#include <limits.h>
	
using namespace std;
using namespace Arts;

// 0: Dir ok
// 1: Dir not ok
static int check_tmp_dir(const char *tmp_dir)
{
  int result;
  struct stat stat_buf;
  result = lstat(tmp_dir, &stat_buf);
  
if ((result == -1) && (errno == ENOENT))
  {
    result = mkdir(tmp_dir, 0700);
    if (result == -1)
    {
       arts_warning("[mcoputils] Error: Cannot create directory \"%s\".\n", tmp_dir);
       return 1;
    }
    result = stat(tmp_dir, &stat_buf);
  }
  if ((result == -1) || (!S_ISDIR(stat_buf.st_mode)))
  {
     arts_warning("[mcoputils] Error: \"%s\" is not a directory.\n", tmp_dir);
     return 1;
  }

  if (stat_buf.st_uid != getuid())
  {
     arts_warning("[mcoputils] Error: \"%s\" is owned by uid %d instead of uid %d.\n", tmp_dir, stat_buf.st_uid, getuid());
     return 1;
  }
  return 0;
}


// 0: Link not ok
// != 0: location of mcop directory
static char *locate_mcop_dir()
{
  struct passwd *pw_ent;
  string kde_tmp_dir;
  string user_tmp_dir;
  int uid = getuid();
  const char *home_dir = getenv("HOME");
  const char *kde_home = uid ? getenv("TDEHOME") : getenv("TDEROOTHOME");
  const char *kde_prefix = "/socket-";
  const char *tmp;
  char *tmp_buf;
  int result;
  struct stat stat_buf;

  tmp = getenv("TDETMP");
  if (!tmp || !tmp[0])
     tmp = getenv("TMPDIR");
  if (!tmp || !tmp[0])
     tmp = "/tmp";
          
  kde_tmp_dir = "";

  pw_ent = getpwuid(uid);
  if (!pw_ent)
  {
     arts_warning("[mcoputils] Error: Can not find password entry for uid %d.\n", getuid());
     return 0;
  }

  user_tmp_dir = string(tmp) + "/tdesocket-" + string(pw_ent->pw_name);

  if (!kde_home || !kde_home[0])
  {
     kde_home = "~/.trinity/";
  }

  if (kde_home[0] == '~')
  {
     if (uid == 0)
     {
        home_dir = pw_ent->pw_dir ? pw_ent->pw_dir : "/root";
     }
     if (!home_dir || !home_dir[0])
     {
        arts_fatal("Aborting. $HOME not set!");
     }
     kde_home++;
     kde_tmp_dir = string(home_dir);
  }
  kde_tmp_dir += kde_home;

  /** Strip trailing '/' **/
  if ( kde_tmp_dir[kde_tmp_dir.length()-1] == '/')
     kde_tmp_dir.resize(kde_tmp_dir.length()-1);

  result = stat(kde_tmp_dir.c_str(), &stat_buf);
  if (result == -1)
  {
     return 0;
  }  

  kde_tmp_dir += kde_prefix;
  {
     char buf[1024];
     if (gethostname(buf, sizeof(buf)-1) != 0)
     {
        arts_fatal("Aborting. Could not determine hostname or hostname too long.");
     }
     buf[sizeof(buf)-1] = '\0';
     kde_tmp_dir += buf;
  }

  result = lstat(kde_tmp_dir.c_str(), &stat_buf);
  if ((result == 0) && (S_ISDIR(stat_buf.st_mode)))
  {
     /* $TDEHOME/socket-$HOSTNAME is a normal directory. Do nothing. */
     tmp_buf = strdup(kde_tmp_dir.c_str());
     return tmp_buf;
  }
  
  if ((result == -1) && (errno == ENOENT))
  {
     // Link mising...
     return 0; 
  }
  if ((result == -1) || (!S_ISLNK(stat_buf.st_mode)))
  {
     arts_warning("[mcoputils] Error: \"%s\" is not a link or a directory.\n", kde_tmp_dir.c_str());
     return 0;
  }

  /* kde_tmp_dir is a link. Check whether it points to a valid directory. */
  ssize_t size = 2048;
  tmp_buf = NULL;
  do {
     size *= 2;
     tmp_buf = (char *) realloc(tmp_buf, size);
     if (!tmp_buf)
	return 0;
     result = readlink(kde_tmp_dir.c_str(), tmp_buf, size - 1);
     if (result == -1)
     {
	arts_warning("[mcoputils] Error: \"%s\" could not be read.\n", kde_tmp_dir.c_str());
	free(tmp_buf);
	return 0;
     }
  } while(result == size - 1);
  tmp_buf[result] = '\0';  

//  printf("[mcoputils] Link \"%s\" points to \"%s\"\n", kde_tmp_dir.c_str(), tmp_buf);
  if (strncmp(tmp_buf, user_tmp_dir.c_str(), user_tmp_dir.length()) != 0)
  {
     arts_warning("[mcoputils] Error: \"%s\" points to \"%s\" instead of \"%s\".\n", kde_tmp_dir.c_str(), tmp_buf, user_tmp_dir.c_str());
     free(tmp_buf);
     return 0;
  }
  result = check_tmp_dir(tmp_buf);
  if (result == 0) return tmp_buf; /* Success */

  free(tmp_buf);
  return 0;
}

/* blatant and literal copy from lnusertemp to avoid tdelibs dependency */
/* Copyright (c) 2000 Waldo Bastian <bastian@kde.org>, released under LGPL */

static
int create_link(const char *file, const char *tmp_dir)
{
  int result;
  result = check_tmp_dir(tmp_dir);
  if (result)
  {
     return result;
  }
  result = symlink(tmp_dir, file);
  if (result == -1)
  {
     fprintf(stderr, "[mcoputils] Error: Can not create link from \"%s\" to \"%s\"\n", file, tmp_dir);
     return 1;
  }
  printf("[mcoputils] Created link from \"%s\" to \"%s\"\n", file, tmp_dir);
  return 0;
}


static
int build_link(string tmp_prefix, const char *kde_prefix)
{
  struct passwd *pw_ent;
  string kde_tmp_dir;
  string user_tmp_dir;
  char *tmp_buf;
  int uid = getuid();
  const char *home_dir = getenv("HOME");
  const char *kde_home = uid ? getenv("TDEHOME") : getenv("TDEROOTHOME");
  int result;
  struct stat stat_buf;

  kde_tmp_dir = "";

  pw_ent = getpwuid(uid);
  if (!pw_ent)
  {
     fprintf(stderr, "[mcoputils] Error: Can not find password entry for uid %d.\n", getuid());
     return 1;
  }

  user_tmp_dir = tmp_prefix + string(pw_ent->pw_name);

  if (!kde_home || !kde_home[0])
  {
     kde_home = "~/.trinity/";
  }

  if (kde_home[0] == '~')
  {
     if (uid == 0)
     {
        home_dir = pw_ent->pw_dir ? pw_ent->pw_dir : "/root";
     }
     if (!home_dir || !home_dir[0])
     {
        fprintf(stderr, "[mcoputils] Aborting. $HOME not set!");
        exit(255);
     }
     kde_home++;
     kde_tmp_dir = string(home_dir);
  }
  kde_tmp_dir += kde_home;

  /** Strip trailing '/' **/
  if ( kde_tmp_dir[kde_tmp_dir.length()-1] == '/')
     kde_tmp_dir.resize(kde_tmp_dir.length()-1);

  result = stat(kde_tmp_dir.c_str(), &stat_buf);
  if ((result == -1) && (errno == ENOENT))
  {
     result = mkdir(kde_tmp_dir.c_str(), 0700);
  }
  if (result == -1)
  {
     return 1;
  }

  kde_tmp_dir += kde_prefix;
  {
     char buf[1024];
     if (gethostname(buf, sizeof(buf)-1) != 0)
     {
        arts_fatal("Aborting. Could not determine hostname or hostname too long.");
     }
     buf[sizeof(buf)-1] = '\0';
     kde_tmp_dir += buf;
  }

  result = lstat(kde_tmp_dir.c_str(), &stat_buf);
  if ((result == 0) && (S_ISDIR(stat_buf.st_mode)))
  {
     /* $TDEHOME/tmp is a normal directory. Do nothing. */
     printf("[mcoputils] Directory \"%s\" already exists.\n", kde_tmp_dir.c_str());
     return 0;
  }
  if ((result == -1) && (errno == ENOENT))
  {
     printf("[mcoputils] Creating link %s.\n", kde_tmp_dir.c_str());
     result = create_link(kde_tmp_dir.c_str(), user_tmp_dir.c_str());
     if (result == 0) return 0; /* Success */
     unlink(kde_tmp_dir.c_str());
     user_tmp_dir += "XXXXXX";
     tmp_buf = strdup(user_tmp_dir.c_str());
     if (mkdtemp(tmp_buf) == NULL)
	 return 1;
     result = create_link(kde_tmp_dir.c_str(), tmp_buf);
     free(tmp_buf);
     return result;
  }
  if ((result == -1) || (!S_ISLNK(stat_buf.st_mode)))
  {
     fprintf(stderr, "[mcoputils] Error: \"%s\" is not a link or a directory.\n", kde_tmp_dir.c_str());
     return 1;
  }
  /* kde_tmp_dir is a link. Check whether it points to a valid directory. */
  ssize_t size = 2048;
  tmp_buf = NULL;
  do {
     size *= 2;
     tmp_buf = (char *) realloc(tmp_buf, size);
     if (!tmp_buf)
	return 0;
     result = readlink(kde_tmp_dir.c_str(), tmp_buf, size - 1);
     if (result == -1)
     {
	arts_warning("[mcoputils] Error: \"%s\" could not be read.\n", kde_tmp_dir.c_str());
	free(tmp_buf);
	return 0;
     }
  } while(result == size - 1);
  tmp_buf[result] = '\0';

  printf("[mcoputils] Link \"%s\" points to \"%s\"\n", kde_tmp_dir.c_str(), tmp_buf);
  if (strncmp(tmp_buf, user_tmp_dir.c_str(), user_tmp_dir.length()) != 0)
  {
     fprintf(stderr, "[mcoputils] Error: \"%s\" points to \"%s\" instead of \"%s\".\n", kde_tmp_dir.c_str(), tmp_buf, user_tmp_dir.c_str());
     free(tmp_buf);
     unlink(kde_tmp_dir.c_str());
     printf("[mcoputils] Creating link %s.\n", kde_tmp_dir.c_str());
     result = create_link(kde_tmp_dir.c_str(), user_tmp_dir.c_str());
     if (result == 0) return 0; /* Success */
     unlink(kde_tmp_dir.c_str());
     user_tmp_dir += "XXXXXX";
     tmp_buf = strdup(user_tmp_dir.c_str());
     if (mkdtemp(tmp_buf) == NULL)
	 return 1;
     result = create_link(kde_tmp_dir.c_str(), tmp_buf);
     free(tmp_buf);
     return result;
  }
  result = check_tmp_dir(tmp_buf);
  free(tmp_buf);
  if (result == 0) return 0; /* Success */
  unlink(kde_tmp_dir.c_str());
  user_tmp_dir += "XXXXXX";
  tmp_buf = strdup(user_tmp_dir.c_str());
  if (mkdtemp(tmp_buf) == NULL)
      return 1;
  result = create_link(kde_tmp_dir.c_str(), tmp_buf);
  free(tmp_buf);
  return result;
}

string MCOPUtils::createFilePath(string name)
{
	static char *mcop_dir = 0;
	if (!mcop_dir)
	{
	   mcop_dir = locate_mcop_dir();
	}
	if (!mcop_dir)
	{

 	   const char *tmp = 0;
	   tmp = getenv("TDETMP");
           if (!tmp || !tmp[0])
             tmp = getenv("TMPDIR");
           if (!tmp || !tmp[0])
             tmp = "/tmp";

           build_link(string(tmp) + "/tdesocket-", "/socket-");
	   mcop_dir = locate_mcop_dir();
	}
	if (!mcop_dir)
	{
	   arts_fatal("can't create mcop directory");
	}
	string tmpdir = mcop_dir;
	return tmpdir+"/"+name;
}

/*
 * try to figure out full hostname - this is important as every client which
 * tries to connect objects located here will need to be able to resolve that 
 * correctly
 */
string MCOPUtils::getFullHostname()
{
	char buffer[1024];
	string result;
	struct hostent *hp;

	if(gethostname(buffer,1024) == 0) {
		buffer[1023] = '\0';
		result = buffer; 
	} else
		return "localhost";

	/*
	 * if gethostname() isn't a FQDN (guess that by checking whether it
	 * contains a dot), try to look it up to ensure it is
	 */
	if(result.find('.') == string::npos && (hp = gethostbyname(buffer)) != 0)
		result = hp->h_name;
	
	return result;
}

string MCOPUtils::readConfigEntry(const string& key, const string& defaultValue)
{
	const char *home = getenv("HOME");
	if(home)
	{
		string rcname = home + string("/.mcoprc");

		MCOPConfig config(rcname);
		return config.readEntry(key,defaultValue);
	}
	return defaultValue;
}

/** IID generation **/

unsigned long MCOPUtils::makeIID(const string& interfaceName)
{
	static map<string, unsigned long> *iidmapobj = 0;
	static unsigned long nextiid = 1;

	if(!iidmapobj) iidmapobj = new map<string,unsigned long>;

	map<string,unsigned long>& iidmap = *iidmapobj;

	if(iidmap.find(interfaceName) == iidmap.end()) {
		iidmap[interfaceName] = nextiid++;
		return nextiid-1; // no need to lookup in the map thrice
	}

	return iidmap[interfaceName];
}

static vector<string> *readPath(const string& name, const string& defaultValue)
{
	vector<string> *result = 0;
	const char *home = getenv("HOME");

	if(home)
	{
		string rcname = home + string("/.mcoprc");

		MCOPConfig config(rcname);
		result = config.readListEntry(name);
	}
	if(!result)
		result = new vector<string>;
	if(result->empty())
		result->push_back(defaultValue);

	return result;
}

const vector<string> *MCOPUtils::extensionPath()
{
	static vector<string> *result = 0;

	if(!result) result = readPath("ExtensionPath", EXTENSION_DIR);
	return result;
}

const vector<string> *MCOPUtils::traderPath()
{
	static vector<string> *result = 0;

	if(!result)
	{
		result = readPath("TraderPath", TRADER_DIR);

		const char *home = getenv("HOME");
		if(home) result->push_back(home + string("/.mcop/trader-cache"));
	}
	return result;
}

string MCOPUtils::mcopDirectory()
{
	static bool initialized = false;
	static string mcopDirectory;

	if(initialized)
		return mcopDirectory;
	initialized = true;

	const char *home = getenv("HOME");
	arts_return_val_if_fail(home != 0, "");

	mcopDirectory = home + string("/.mcop");

	mkdir(home,0755);
	if(mkdir(mcopDirectory.c_str(),0755) != 0)
	{
		string why = strerror(errno);

		struct stat st;
		stat(mcopDirectory.c_str(),&st);
		if(!S_ISDIR(st.st_mode))
		{
			arts_warning("[mcoputils] can't create directory %s (%s)",
					 	 mcopDirectory.c_str(), why.c_str());

			mcopDirectory = "";
		}
	}
	return mcopDirectory;
}

bool MCOPUtils::tokenize(const string& line, string& key,vector<string>& values)
{
	string value;
	enum { sKey, sValue, sValueQuoted, sValueQuotedEscaped, sBad } state;

	state = sKey;
	for(string::const_iterator i = line.begin(); i != line.end(); i++)
	{
		char c = *i;
		unsigned char uc = static_cast<unsigned char>(c);

		arts_assert(c != '\n');

		if(state == sKey)
		{
			if(c == ' ' || c == '\t')
				; // ignore
			else if(isalnum(c))
				key += c;
			else if(c == '=')
				state = sValue;
			else
				state = sBad;
		}
		else if(state == sValue)
		{
			if(c == ' ' || c == '\t')
				; // ignore
			else if(c == '"')
				state = sValueQuoted;
			else if(c == ',')
			{
				values.push_back(value);
				value = "";
			}
			else if(uc > 32 && uc < 128)
				value += c;
			else
				state = sBad;
		}
		else if(state == sValueQuoted)
		{
			if(c == '"')
				state = sValue;
			else if(c == '\\')
				state = sValueQuotedEscaped;
			else
				value += c;
		}
		else if(state == sValueQuotedEscaped)
		{
			value += c;
			state = sValueQuoted;
		}
	}
	if(state == sValue)
		values.push_back(value);

	return(state != sBad);
}
