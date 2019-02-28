

#ifndef _JSON_H_
#define _JSON_H_

#include "Export.h"
#include <stddef.h>
#include "json.h"

#include <algorithm> // sort
#include <stdio.h>


#if defined(_MSC_VER)  &&  _MSC_VER >= 1310
# pragma warning( disable: 4996 )     // disable fopen deprecation warning
#endif



	static std::string
		readInputTestFile( const char *path )
	{
		FILE *file = fopen( path, "rb" );
		if ( !file )
			return std::string("");
		fseek( file, 0, SEEK_END );
		long size = ftell( file );
		fseek( file, 0, SEEK_SET );
		std::string text;
		char *buffer = new char[size+1];
		buffer[size] = 0;
		if ( fread( buffer, 1, size, file ) == (unsigned long)size )
			text = buffer;
		fclose( file );
		delete[] buffer;
		return text;
	}
	static void
		printValueTree( FILE *fout, Json::Value &value, const std::string &path = "." )
	{
		switch ( value.type() )
		{
		case Json::nullValue:
			fprintf( fout, "%s=null\n", path.c_str() );
			break;
		case Json::intValue:
			fprintf( fout, "%s=%d\n", path.c_str(), value.asInt() );
			break;
		case Json::uintValue:
			fprintf( fout, "%s=%u\n", path.c_str(), value.asUInt() );
			break;
		case Json::realValue:
			fprintf( fout, "%s=%.16g\n", path.c_str(), value.asDouble() );
			break;
		case Json::stringValue:
			fprintf( fout, "%s=\"%s\"\n", path.c_str(), value.asString().c_str() );
			break;
		case Json::booleanValue:
			fprintf( fout, "%s=%s\n", path.c_str(), value.asBool() ? "true" : "false" );
			break;
		case Json::arrayValue:
			{
				fprintf( fout, "%s=[]\n", path.c_str() );
				int size = value.size();
				for ( int index =0; index < size; ++index )
				{
					static char buffer[16];
					sprintf( buffer, "[%d]", index );
					printValueTree( fout, value[index], path + buffer );
				}
			}
			break;
		case Json::objectValue:
			{
				fprintf( fout, "%s={}\n", path.c_str() );
				Json::Value::Members members( value.getMemberNames() );
				std::sort( members.begin(), members.end() );
				std::string suffix = *(path.end()-1) == '.' ? "" : ".";
				for ( Json::Value::Members::iterator it = members.begin(); 
					it != members.end(); 
					++it )
				{
					const std::string &name = *it;
					printValueTree( fout, value[name], path + suffix + name );
				}
			}
			break;
		default:
			break;
		}
	}
	static void
		printValueTree( Json::Value &value, const std::string &path = "." )
	{
		switch ( value.type() )
		{
		case Json::nullValue:
			printf(  "%s=null\n", path.c_str() );
			break;
		case Json::intValue:
			printf(  "%s=%d\n", path.c_str(), value.asInt() );
			break;
		case Json::uintValue:
			printf(  "%s=%u\n", path.c_str(), value.asUInt() );
			break;
		case Json::realValue:
			printf(  "%s=%.16g\n", path.c_str(), value.asDouble() );
			break;
		case Json::stringValue:
			printf(  "%s=\"%s\"\n", path.c_str(), value.asString().c_str() );
			break;
		case Json::booleanValue:
			printf(  "%s=%s\n", path.c_str(), value.asBool() ? "true" : "false" );
			break;
		case Json::arrayValue:
			{
				printf(  "%s=[]\n", path.c_str() );
				int size = value.size();
				for ( int index =0; index < size; ++index )
				{
					static char buffer[16];
					sprintf( buffer, "[%d]", index );
					printValueTree(  value[index], path + buffer );
				}
			}
			break;
		case Json::objectValue:
			{
				printf(  "%s={}\n", path.c_str() );
				Json::Value::Members members( value.getMemberNames() );
				std::sort( members.begin(), members.end() );
				std::string suffix = *(path.end()-1) == '.' ? "" : ".";
				for ( Json::Value::Members::iterator it = members.begin(); 
					it != members.end(); 
					++it )
				{
					const std::string &name = *it;
					printValueTree(  value[name], path + suffix + name );
				}
			}
			break;
		default:
			break;
		}
	}


	static int
		parseAndSaveValueTree( const std::string &input, 
		const std::string &actual,
		const std::string &kind,
		Json::Value &root )
	{
		Json::Reader reader;
		bool parsingSuccessful = reader.parse( input, root );
		if ( !parsingSuccessful )
		{
			printf( "Failed to parse %s file: \n%s\n", 
				kind.c_str(),
				reader.getFormatedErrorMessages().c_str() );
			return 1;
		}

		FILE *factual = fopen( actual.c_str(), "wt" );
		if ( !factual )
		{
			printf( "Failed to create %s actual file.\n", kind.c_str() );
			return 2;
		}
		printValueTree(root );
		fclose( factual );
		return 0;
	}


	static int
		rewriteValueTree( const std::string &rewritePath, 
		const Json::Value &root, 
		std::string &rewrite )
	{
		//Json::FastWriter writer;
		//writer.enableYAMLCompatibility();
		Json::StyledWriter writer;
		rewrite = writer.write( root );
		FILE *fout = fopen( rewritePath.c_str(), "wt" );
		if ( !fout )
		{
			printf( "Failed to create rewrite file: %s\n", rewritePath.c_str() );
			return 2;
		}
		fprintf( fout, "%s\n", rewrite.c_str() );
		fclose( fout );
		return 0;
	}


	static std::string
		removeSuffix( const std::string &path, 
		const std::string &extension )
	{
		if ( extension.length() >= path.length() )
			return std::string("");
		std::string suffix = path.substr( path.length() - extension.length() );
		if ( suffix != extension )
			return std::string("");
		return path.substr( 0, path.length() - extension.length() );
	}


#endif




//
//int main( int argc, const char *argv[] )
//{
//   if ( argc != 2 )
//   {
//      printf( "Usage: %s input-json-file", argv[0] );
//      return 3;
//   }
//
//   std::string input = readInputTestFile( argv[1] );
//   if ( input.empty() )
//   {
//      printf( "Failed to read input or empty input: %s\n", argv[1] );
//      return 3;
//   }
//
//   std::string basePath = removeSuffix( argv[1], ".json" );
//   if ( basePath.empty() )
//   {
//      printf( "Bad input path. Path does not end with '.expected':\n%s\n", argv[1] );
//      return 3;
//   }
//
//   std::string actualPath = basePath + ".actual";
//   std::string rewritePath = basePath + ".rewrite";
//   std::string rewriteActualPath = basePath + ".actual-rewrite";
//
//   Json::Value root;
//   int exitCode = parseAndSaveValueTree( input, actualPath, "input", root );
//   if ( exitCode == 0 )
//   {
//      std::string rewrite;
//      exitCode = rewriteValueTree( rewritePath, root, rewrite );
//      if ( exitCode == 0 )
//      {
//         Json::Value rewriteRoot;
//         exitCode = parseAndSaveValueTree( rewrite, rewriteActualPath, "rewrite", rewriteRoot );
//      }
//   }
//
//   return exitCode;
//}

