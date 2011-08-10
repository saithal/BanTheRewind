/*
* 
* Copyright (c) 2011, Ban the Rewind
* All rights reserved.
* 
* Redistribution and use in source and binary forms, with or 
* without modification, are permitted provided that the following 
* conditions are met:
* 
* Redistributions of source code must retain the above copyright 
* notice, this list of conditions and the following disclaimer.
* Redistributions in binary form must reproduce the above copyright 
* notice, this list of conditions and the following disclaimer in 
* the documentation and/or other materials provided with the 
* distribution.
* 
* Neither the name of the Ban the Rewind nor the names of its 
* contributors may be used to endorse or promote products 
* derived from this software without specific prior written 
* permission.
* 
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS 
* FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE 
* COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, 
* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
* BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, 
* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
* ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
* ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
* 
*/

// Include header
#include "ciJson.h"

namespace cinder
{

	namespace json
	{

		// Add values
		void append(Value & object, const string & key, bool value) { object[key] = Value(value); }
		void append(Value & object, const string & key, char * value) { object[key] = Value(value); }
		void append(Value & object, const string & key, double value) { object[key] = Value(value); }
		void append(Value & object, const string & key, float value) { object[key] = Value(value); }
		void append(Value & object, const string & key, int32_t value) { object[key] = Value(value); }
		void append(Value & object, const string & key, string value) { object[key] = Value(value.c_str()); }
		void append(Value & object, const string & key, uint32_t value) { object[key] = Value(value); }

		// Append list of values
		void append(Value & object, const string & key, vector<bool> values)
		{
			for (vector<bool>::const_iterator mMember = values.begin(); mMember != values.end(); ++mMember)
				object[key].append(* mMember);
		}
		void append(Value & object, const string & key, vector<double> values)
		{
			for (vector<double>::const_iterator mMember = values.begin(); mMember != values.end(); ++mMember)
				object[key].append(* mMember);
		}
		void append(Value & object, const string & key, vector<float> values)
		{
			for (vector<float>::const_iterator mMember = values.begin(); mMember != values.end(); ++mMember)
				object[key].append(* mMember);
		}
		void append(Value & object, const string & key, vector<int32_t> values)
		{ 
			for (vector<int32_t>::const_iterator mMember = values.begin(); mMember != values.end(); ++mMember)
				object[key].append(* mMember);
		}
		void append(Value & object, const string & key, vector<string> values) 
		{ 
			for (vector<string>::const_iterator mMember = values.begin(); mMember != values.end(); ++mMember)
				object[key].append(* mMember);
		}
		void append(Value & object, const string & key, vector<uint32_t> values)
		{ 
			for (vector<uint32_t>::const_iterator mMember = values.begin(); mMember != values.end(); ++mMember)
				object[key].append(* mMember);
		}
		void append(Value & object, const string & key, vector<Value> values) 
		{
			for (vector<Value>::const_iterator mMember = values.begin(); mMember != values.end(); ++mMember)
				object[key].append(* mMember);
		}

		// Deserialize JSON string to object
		Value deserialize(const string & value)
		{

			// Parse and return data
			Json::Reader mReader;
			Json::Value mRoot;
			mReader.parse(value, mRoot);
			return mRoot;

		}

		// Load JSON from file
		Value load(const string & filename)
		{

			// Open config file, starting from end
			fstream mFile;
			mFile.open(filename.c_str(), ios::in|ios::ate);
			if (!mFile.is_open())
			{
				Value value;
				return value;
			}

			// Get file size and rewind to beginning
			ifstream::pos_type mSize = mFile.tellg();
			char * mConfigData = new char[mSize];
			mFile.seekg(0, ios::beg);

			// Read entire file into JSON data
			mFile.read(mConfigData, mSize);
			Value mValue = deserialize(string(mConfigData));

			// Clean up
			delete [] mConfigData;
			mFile.close();

			// Return root
			return mValue;

		}

		// Load JSON from URL
		Value loadUrl(const string & url, bool isEscaped)
		{

			// Load URL
			DataSourceRef mDataSource = ci::loadUrl(Url(url, isEscaped));

			// Read buffer
			Buffer mBuffer;
			try
			{
				mBuffer = mDataSource->getBuffer();
			}
			catch (...)
			{
				Value value;
				return value;
			}
			size_t mDataSize = mBuffer.getDataSize();
			shared_ptr<int_fast8_t> mBufferStr(new int_fast8_t[mDataSize + 1], checked_array_deleter<int_fast8_t>());
			memcpy(mBufferStr.get(), mBuffer.getData(), mBuffer.getDataSize());
			mBufferStr.get()[mDataSize] = 0;

			// Convert buffer to string, deserialize and return
			return deserialize(string(reinterpret_cast<char *>(mBufferStr.get())));

		}

		// Read values (index)
		bool readBool(Value & object, int32_t index) { return object[index].asBool(); }
		double readDouble(Value & object, int32_t index) { return object[index].asDouble(); }
		int32_t readInt(Value & object, int32_t index) { return object[index].asInt(); }
		const char * readCString(Value & object, int32_t index) { return object[index].asCString(); }
		string readString(Value & object, int32_t index) { return object[index].asString(); }
		uint32_t readUint(Value & object, int32_t index) { return object[index].asUInt(); }
		Value readValue(Value & object, int32_t index) { return object[index]; }

		// Read values (index, plural)
		vector<bool> readBools(Value & object, int32_t index) 
		{
			vector<bool> mMembers;
			for (uint32_t i = 0; i < object[index].size(); i++)
				mMembers.push_back(object[index][i].asBool());
			return mMembers;
		}
		vector<double> readDoubles(Value & object, int32_t index) 
		{
			vector<double> mMembers;
			for (uint32_t i = 0; i < object[index].size(); i++)
				mMembers.push_back(object[index][i].asDouble());
			return mMembers;
		}
		vector<int32_t> readInts(Value & object, int32_t index) 
		{
			vector<int32_t> mMembers;
			for (uint32_t i = 0; i < object[index].size(); i++)
				mMembers.push_back(object[index][i].asInt());
			return mMembers;
		}
		vector<string> readStrings(Value & object, int32_t index) 
		{
			vector<string> mMembers;
			for (uint32_t i = 0; i < object[index].size(); i++)
				mMembers.push_back(object[index][i].asString());
			return mMembers;
		}
		vector<uint32_t> readUints(Value & object, int32_t index) 
		{
			vector<uint32_t> mMembers;
			for (uint32_t i = 0; i < object[index].size(); i++)
				mMembers.push_back(object[index][i].asUInt());
			return mMembers;
		}
		vector<Value> readValues(Value & object, int32_t index) 
		{ 
			vector<Value> mMembers;
			for (uint32_t i = 0; i < object[index].size(); i++)
				mMembers.push_back(object[index][i]);
			return mMembers;
		}

		// Read values (key)
		bool readBool(Value & object, const string & key) { return object[key].asBool(); }
		double readDouble(Value & object, const string & key) { return object[key].asDouble(); }
		int32_t readInt(Value & object, const string & key) { return object[key].asInt(); }
		string readString(Value & object, const string & key) { return object[key].asString(); }
		uint32_t readUint(Value & object, const string & key) { return object[key].asUInt(); }
		Value readValue(Value & object, const string & key) { return object[key]; }

		// Read values (key, plural)
		vector<bool> readBools(Value & object, const string & key) 
		{
			vector<bool> mMembers;
			for (uint32_t i = 0; i < object[key].size(); i++)
				mMembers.push_back(object[key][i].asBool());
			return mMembers;
		}
		vector<double> readDoubles(Value & object, const string & key) 
		{
			vector<double> mMembers;
			for (uint32_t i = 0; i < object[key].size(); i++)
				mMembers.push_back(object[key][i].asDouble());
			return mMembers;
		}
		vector<int32_t> readInts(Value & object, const string & key) 
		{
			vector<int32_t> mMembers;
			for (uint32_t i = 0; i < object[key].size(); i++)
				mMembers.push_back(object[key][i].asInt());
			return mMembers;
		}
		vector<string> readStrings(Value & object, const string & key) 
		{
			vector<string> mMembers;
			for (uint32_t i = 0; i < object[key].size(); i++)
				mMembers.push_back(object[key][i].asString());
			return mMembers;
		}
		vector<uint32_t> readUints(Value & object, const string & key) 
		{
			vector<uint32_t> mMembers;
			for (uint32_t i = 0; i < object[key].size(); i++)
				mMembers.push_back(object[key][i].asUInt());
			return mMembers;
		}
		vector<Value> readValues(Value & object, const string & key) 
		{ 
			vector<Value> mMembers;
			for (uint32_t i = 0; i < object[key].size(); i++)
				mMembers.push_back(object[key][i]);
			return mMembers;
		}

		// Save JSON from file
		void save(const Value & value, const string & filename)
		{

			// Serialize data into string
			string mJsonStr = serialize(value);

			// Write JSON to file
			fstream mFile;
			mFile.open(filename.c_str(), ios::out|ios::trunc);
			mFile << mJsonStr;
			mFile.close();

		}

#if defined(CINDER_MSW)

		// POST JSON to URL
		string saveUrl(const Value & value, const string & url)
		{

			// Tokenize URL
			vector<string> mTokens;
			boost::split(mTokens, url, boost::is_any_of("/"));
			if (mTokens.size() < 3)
				return "Invalid URL. Please include protocol.";

			// Get server portion of URL
			string mServer = mTokens[2];

			// Remove server portion of URL
			for (int32_t i = 0; i < 3; i++)
				mTokens.erase(mTokens.begin());

			// Get action portion of URL
			string mAction = "";
			for (vector<string>::const_iterator mToken = mTokens.begin(); mToken != mTokens.end(); ++mToken)
				mAction += "/" + * mToken;

			// Serialize data
			string mData = serialize(value);

			// Open Internet connection
			HINTERNET mSession = InternetOpenA("WinInetConnection", INTERNET_OPEN_TYPE_PRECONFIG_WITH_NO_AUTOPROXY, NULL, NULL, 0);
			if (mSession == NULL)
				return "Unable to establish Internet session.";

			// Open server
			HINTERNET mConnect = InternetConnectA(mSession, mServer.c_str(), INTERNET_DEFAULT_HTTP_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, 0, NULL);
			if (mConnect == NULL)
				return "Unable to establish Internet connection.";

			// Open request flags
			DWORD mOpenRequestFlags = INTERNET_FLAG_IGNORE_REDIRECT_TO_HTTP |
				INTERNET_FLAG_IGNORE_REDIRECT_TO_HTTPS |
				INTERNET_FLAG_KEEP_CONNECTION |
				INTERNET_FLAG_NO_AUTO_REDIRECT |
				INTERNET_FLAG_NO_COOKIES |
				INTERNET_FLAG_NO_CACHE_WRITE |
				INTERNET_FLAG_NO_UI |
				INTERNET_FLAG_RELOAD;

			// Open request
			HINTERNET mRequest = HttpOpenRequestA(mConnect, "POST", mAction.c_str(), "HTTP/1.0", NULL, NULL, mOpenRequestFlags, 0);
			if (mRequest == NULL)
				return "Unable to create request.";

			// Send request
			int_fast8_t mBuffer;
			DWORD mSize;
			string mHeaders = "Content-Type: application/x-www-form-urlencoded";
			string mResponse = "";
			if (HttpSendRequestA(mRequest, mHeaders.c_str(), mHeaders.length(), (LPVOID)(mData.c_str()), mData.length()))
			{

				// Read request into buffer
				while (InternetReadFile(mRequest, &mBuffer, 1, &mSize))
				{
					if (mSize != 1)
						break;
					mResponse += mBuffer;
				}

			}

			// Close Internet handles
			InternetCloseHandle(mRequest);
			InternetCloseHandle(mConnect);
			InternetCloseHandle(mSession);

			// Return response
			return mResponse;

		}
#endif

		// Serialize JSON object to string
		string serialize(const Json::Value & value)
		{	

			// Writes JSON
			Json::StyledWriter mWriter;

			// Convert value to stylized string and return
			return mWriter.write(value);

		}

	}

}
