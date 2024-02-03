#include <iostream>
#include <fstream>
using namespace std;
class Activity;
class Date;
class Memory;
class Comment;
class Post;
class Object;
class SocialMedia;
class Page;
class User;

class Helper
{
public:
	static int StringLength(char* str)
	{
		int length = 0;

		for (int i = 0; str[i] != '\0'; i++)
		{
			length++;
		}

		return length;
	}
	static void StringCopy(char*& dest, char*& src)		//For deep copy of string
	{
		int i = 0;

		while (src[i] != '\0')
		{
			dest[i] = src[i];
			i++;
		}

		dest[i] = '\0';
	}
	static char* GetStringFromBuffer(char* str)		//allocates exact size of str on heap and returns pointer to that
	{
		int size = StringLength(str);

		char* newTemp = new char[size + 1];
		StringCopy(newTemp, str);

		return newTemp;
	}
	static bool CompareString(char* str1, char* str2)		//Checks if two strings are same
	{
		if (StringLength(str1) != StringLength(str2))
			return false;

		bool same = true;
		for (int i = 0; str1[i] != '\0' && same; i++)
		{
			if (str1[i] != str2[i])
				same = false;
		}
		return same;
	}
	template<typename T>
	static void Deallocate(T** list, int size)		//Deallocates any array of pointers
	{
		for (int i = 0; i < size; i++)
		{
			delete list[i];
		}
		delete[] list;
	}
	template<typename T>
	static void InitializePointers(T** list, int size)	//Initializes any pointer array with nullptrs
	{
		for (int i = 0; i < size; i++)
		{
			list[i] = nullptr;
		}
	}
	static void StringConcatenation(char*& str1, char* str2)
	{
		int len1 = StringLength(str1);
		int len2 = StringLength(str2);

		char* temp = new char[len1 + len2 + 1];

		for (int i = 0; i < len1; i++)
		{
			temp[i] = str1[i];
		}

		for (int i = 0, j = len1; i < len2; i++)
		{
			temp[i + len1] = str2[i];
		}

		temp[len1 + len2] = '\0';

		delete[] str1;
		str1 = new char[len1 + len2 + 1];

		for (int i = 0; i < len1 + len2; i++)
		{
			str1[i] = temp[i];
		}

		str1[len1 + len2] = '\0';

		delete[] temp;
	}
	static char* CreateNewID(char* letters, int num)
	{
		char buffer[30];
		char* newID = new char[StringLength(letters) + 1];

		StringCopy(newID, letters);
		_itoa_s(num, buffer, 10);
		StringConcatenation(newID, buffer);

		return newID;
	}
};

class Object		//base class for class Page and class User
{
private:
	char* ID;
public:
	Object()
	{
		ID = nullptr;
	}
	~Object()
	{
		if (ID)
			delete[] ID;
	}
	virtual void AddPostToTimeline(Post* ptr) = 0;
	virtual void PrintName() = 0;
	virtual void PrintListView() = 0;
	void SetID(char* objID)
	{
		ID = Helper::GetStringFromBuffer(objID);
	}
	char* GetID()
	{
		return ID;
	}
};

class User : public Object		//derived class of clss Object
{
private:
	char* fName;
	char* lName;
	int likedPagesCount;
	int friendCount;
	Page** likedPages;
	User** friendsList;
	Post** Timeline;
	int timelineCount;

public:
	User()
	{
		fName = nullptr;
		lName = nullptr;
		likedPagesCount = 0;
		likedPages = nullptr;
		friendCount = 0;
		friendsList = nullptr;
		Timeline = nullptr;
		timelineCount = 0;
	}
	~User()
	{
		if (fName)
			delete[] fName;
		if (lName)
			delete[] lName;
		if (likedPages)
			delete[] likedPages;
		if (friendsList)
			delete[] friendsList;
		if (Timeline)
			Helper::Deallocate(Timeline, timelineCount);
	}
	void AddFriend(User* friendToAdd)		//for association of friend User with User
	{
		if (!friendsList)
		{
			friendsList = new User * [10];
			Helper::InitializePointers(friendsList, 10);
		}

		if (friendCount < 10)
		{
			friendsList[friendCount] = friendToAdd;
			friendCount++;
		}
	}
	void LikePage(Page* pageToLike)		//for association of page with user
	{
		if (!likedPages)
		{
			likedPages = new Page * [10];
			Helper::InitializePointers(likedPages, 10);
		}

		if (likedPagesCount < 10)
		{
			likedPages[likedPagesCount] = pageToLike;
			likedPagesCount++;
		}
	}
	void AddPostToTimeline(Post* ptr)		//for association ofshared post with user
	{
		if (!Timeline)
		{
			Timeline = new Post * [10];
			Helper::InitializePointers(Timeline, 10);
		}

		if (timelineCount < 10)
		{
			Timeline[timelineCount] = ptr;
			timelineCount++;
		}
	}
	void LikePost(Post* postPtr);		//for association of liked post with user
	void PostComment(Post* postPtr, char* text);		// To post a comment on a post
	void ReadDataFromFile(istream& fin)
	{
		char buffer[50];

		fin >> buffer;
		SetID(buffer);

		fin >> buffer;
		fName = Helper::GetStringFromBuffer(buffer);

		fin >> buffer;
		lName = Helper::GetStringFromBuffer(buffer);
	}
	void ShareMemory(Post* postPtr, char* memoryText);		//Shares memory by calling AddToMemory(postPtr) function of memory
	void PrintFriendsList()		//prints list of all friends
	{
		cout << fName << " " << lName << " - Friends List\n\n";
		for (int i = 0; i < friendCount; i++)
		{
			cout << friendsList[i]->GetID() << " - " << friendsList[i]->fName << " " << friendsList[i]->lName << endl;
		}
		cout << endl;
	}
	void PrintName()
	{
		cout << fName << " " << lName;
	}
	void PrintListView()
	{
		cout << GetID() << " - " << fName << " " << lName << endl;
	}
	void ViewHome();		//Prints all posts shared by friends of user within 24 hours
	void ViewTimeline();	//Prints all shared posts of users
	void PrintLikedPageList();	//Prints a list of all liked pages
	void ViewSharedPostsByCurrentDate();	//Called in ViewHome to view posts shared within 24 hours
	void SeeYourMemories();	//Prints posts that are 1 year or more old	
};

class Page : public Object	//derived class of class Object
{
private:
	char* title;
	Post** Timeline;
	int timelineCount;
public:
	Page()
	{
		title = nullptr;
		Timeline = nullptr;
		timelineCount = 0;
	}
	~Page()
	{
		if (title)
			delete[] title;
		if (Timeline)
			Helper::Deallocate(Timeline, timelineCount);
	}
	void ReadDataFromFile(istream& fin)
	{
		char buffer[100];

		fin >> buffer;
		SetID(buffer);

		fin.getline(buffer, 100);

		title = new char[Helper::StringLength(buffer)];
		int j = 0;

		for (int i = 0; buffer[i] != '\0'; i++)
		{
			if (buffer[i] != '\t')
			{
				title[j] = buffer[i];
				j++;
			}
		}
		title[j] = '\0';
	}
	void AddPostToTimeline(Post* ptr)		//adding posts shared by page into timeline through association
	{
		if (!Timeline)
		{
			Timeline = new Post * [10];
			Helper::InitializePointers(Timeline, 10);
		}

		if (timelineCount < 10)
		{
			Timeline[timelineCount] = ptr;
			timelineCount++;
		}
	}
	void PrintName()
	{
		cout << title;
	}
	void PrintListView()
	{
		cout << GetID() << " - " << title << endl;
	}
	void ViewSharedPostsByCurrentDate();		//Prints posts shared within 24 hours
	void ViewSharedPosts();		//Prints all shared posts by page
};

class Date
{
private:
	int day;
	int month;
	int year;
public:
	static Date currentDate;
	Date(int d = 0, int m = 0, int y = 0)
	{
		day = d;
		month = m;
		year = y;
	}
	~Date()
	{

	}
	static void SetCurrentDate()
	{
		currentDate.day = 15;
		currentDate.month = 11;
		currentDate.year = 2017;
	}
	static void PrintCurrentDate()
	{
		cout << "System Date:\t";
		cout << currentDate.day << "/" << currentDate.month << "/" << currentDate.year << endl << endl;
	}
	void ReadDataFromFile(istream& fin)
	{
		fin >> day;
		fin >> month;
		fin >> year;
	}
	void Print()
	{
		cout << "(" << day << "/" << month << "/" << year << ")";
	}
	int CalculateYearDifference()		//returns years difference of Date object in this and currentDate Date object
	{
		if (day == currentDate.day)
		{
			if (currentDate.month == month)
			{
				if (currentDate.year - year >= 1)
					return currentDate.year - year;
				else
					return -1;
			}
			else
				return -1;
		}
		else
			return -1;
	}
	bool CompareDate()		//comapred this with currentDate and returns true if this is equal to currentDate or one day behind
	{
		if (currentDate.year == year)
		{
			if (currentDate.month == month)
			{
				if (currentDate.day == day || currentDate.day == day + 1)
				{
					return true;
				}
				else
				{
					return false;
				}
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}
};

Date Date::currentDate;

class Activity
{
private:
	int type;
	char* value;
public:
	Activity()
	{
		type = 0;
		value = nullptr;
	}
	~Activity()
	{
		if (value)
			delete[] value;
	}
	void ReadDataFromFile(istream& fin)
	{
		char buffer[80];
		char c;

		fin >> type;
		fin.get(c);
		fin.getline(buffer, 80);

		value = Helper::GetStringFromBuffer(buffer);
	}
	void Print()
	{
		cout << " is ";
		if (type == 1)
			cout << "Feeling ";
		else if (type == 2)
			cout << "Thinking About ";
		else if (type == 3)
			cout << "Making ";
		else if (type == 4)
			cout << "Celebrating ";

		cout << value << endl;
	}

};
class Post
{
private:
	Activity* activity;
	char* ID;
	char* text;
	Date SharedDate;
	Object* SharedBy;
	Object** LikedBy;
	Comment** commentList;
	int commentCount;
	int likedByCount;
	static int postCount;

public:
	Post()
	{
		activity = nullptr;
		ID = nullptr;
		text = nullptr;
		SharedBy = nullptr;
		LikedBy = nullptr;
		commentList = nullptr;
		likedByCount = 0;
		commentCount = 0;
		postCount++;
	}
	~Post()
	{
		if (activity)
			delete activity;
		if (ID)
			delete[] ID;
		if (text)
			delete[] text;
		if (LikedBy)
			delete[] LikedBy;
		if (commentList)
			Helper::Deallocate(commentList, commentCount);
	}
	char* GetPostID()
	{
		return ID;
	}
	void ReadDataFromFile(istream& fin)
	{
		int activityCheck;
		char buffer[100];
		char c;

		fin >> activityCheck;
		fin >> buffer;

		ID = Helper::GetStringFromBuffer(buffer);
		SharedDate.ReadDataFromFile(fin);

		fin.get(c);
		fin.getline(buffer, 100);

		text = Helper::GetStringFromBuffer(buffer);

		if (activityCheck == 2)
		{
			activity = new Activity;
			activity->ReadDataFromFile(fin);
		}
	}
	void SetSharedBy(Object* sharedBy)
	{
		SharedBy = sharedBy;
	}
	void SetLikedBy(Object* likedBy)		//adding page or user to list of likedBy through association
	{
		if (!LikedBy)
		{
			LikedBy = new Object * [10];
			Helper::InitializePointers(LikedBy, 10);
		}

		if (likedByCount < 10)
		{
			LikedBy[likedByCount] = likedBy;
			likedByCount++;
		}

	}
	void AddCommentToList(Comment* commentToAdd)		//adding comment on post through association
	{
		if (!commentList)
		{
			commentList = new Comment * [10];
			Helper::InitializePointers(commentList, 10);
		}

		if (commentCount < 10)
		{
			commentList[commentCount] = commentToAdd;
			commentCount++;
		}
	}
	bool SearchInLiked(Object* ptr)
	{
		bool found = false;
		for (int i = 0; i < likedByCount && !found; i++)
		{
			if (LikedBy[i] == ptr)
				found = true;
		}
		return found;
	}
	bool CompareDate()
	{
		if (SharedDate.CompareDate())
			return true;

		return false;
	}
	int CalculateYearDifference()
	{
		return SharedDate.CalculateYearDifference();
	}
	virtual void PrintPost();	//Prints all data of post
	void PrintPostWithoutComment();	//prints all data except comments
	virtual void PrintPostWithoutDate(); //prints all data except shared date
	void ViewLikedList()	//Prints list of pages or users that have liked the post
	{
		cout << "Post liked by:\n";
		for (int i = 0; i < likedByCount; i++)
		{
			LikedBy[i]->PrintListView();
		}
		cout << endl;
	}
	char* GetPostText()
	{
		return text;
	}
	Object* GetSharedByID()
	{
		return SharedBy;
	}
	void SetMemoryVals(char* postText)
	{
		char postLetters[] = "post";
		postCount++;
		ID = Helper::GetStringFromBuffer(Helper::CreateNewID(postLetters, postCount));
		text = Helper::GetStringFromBuffer(postText);
		SharedDate = Date::currentDate;
	}
};
int Post::postCount = 0;

class Comment
{
private:
	char* ID;
	char* commentText;
	Object* commentBy;
	static int count;
public:
	Comment()
	{
		ID = nullptr;
		commentText = nullptr;
		commentBy = nullptr;
		count++;
	}
	~Comment()
	{
		if (ID)
			delete[] ID;
		if (commentText)
			delete[] commentText;
	}
	void SetValues(char* id, char* comment, Object* commenter)
	{
		if (commenter)
			commentBy = commenter;

		commentText = Helper::GetStringFromBuffer(comment);
		ID = Helper::GetStringFromBuffer(id);
	}
	void SetValues(char* comment, Object* commenter)
	{
		char c[] = "c";
		if (commenter)
			commentBy = commenter;

		commentText = Helper::GetStringFromBuffer(comment);
		ID = Helper::GetStringFromBuffer(Helper::CreateNewID(c, count));
	}
	void Print()	//Prints all data of comment
	{
		commentBy->PrintName();
		cout << " wrote: \"" << commentText << "\"";
		cout << endl;
	}
};

int Comment::count = 0;

class Memory : public Post
{
private:
	Post* originalPost;
public:
	Memory()
	{
		originalPost = nullptr;
	}
	Memory(Post* ptr, char* text)
	{
		originalPost = ptr;
		SetMemoryVals(text);
	}
	~Memory()
	{

	}
	void PrintPost()
	{
		cout << "~~~ ";
		originalPost->GetSharedByID()->PrintName();
		cout << " shared a memory ~~~ ...";
		Date::currentDate.Print();
		cout << endl;
		cout << "   \"" << GetPostText() << "\"\n";
		cout << "\t(" << originalPost->CalculateYearDifference() << " years ago)\n";
		originalPost->PrintPostWithoutComment();
	}
	void PrintPostWithoutDate()
	{
		cout << "~~~ ";
		originalPost->GetSharedByID()->PrintName();
		cout << " shared a memory ~~~";
		cout << endl;
		cout << "   \"" << GetPostText() << "\"\n";
		cout << "\t(" << originalPost->CalculateYearDifference() << " years ago)\n";
		originalPost->PrintPostWithoutComment();
	}
};

class SocialMedia
{
private:
	Page** pageList;
	User** userList;
	Post** postList;
	int totalPages;
	int totalUsers;
	int totalPosts;
public:
	SocialMedia()
	{
		totalUsers = 0;
		totalPages = 0;
		totalPosts = 0;
		pageList = nullptr;
		userList = nullptr;
		postList = nullptr;
	}
	~SocialMedia()
	{
		if (pageList)
		{
			Helper::Deallocate(pageList, totalPages);
		}
		if (userList)
		{
			Helper::Deallocate(userList, totalUsers);
		}
		if (postList)
			delete[] postList;
	}
	User* SearchUserByID(char* ID)		//Searches in userList any user ID that is passed and returns its pointer
	{
		bool found = false;
		User* friendSearched = nullptr;
		for (int i = 0; i < totalUsers && !found; i++)
		{
			if (Helper::CompareString(ID, userList[i]->GetID()))
			{
				found = true;
				friendSearched = userList[i];
			}
		}
		return friendSearched;
	}
	Page* SearchPageByID(char* ID)		//Searches in pageList any page ID that is passed and returns its pointer
	{
		bool found = false;
		Page* pageSearched = nullptr;
		for (int i = 0; i < totalPages && !found; i++)
		{
			if (Helper::CompareString(ID, pageList[i]->GetID()))
			{
				found = true;
				pageSearched = pageList[i];
			}
		}
		return pageSearched;
	}
	Object* SearchObjectByID(char* ID)		//Searches in use rList and page list any user ID or page ID that is passed and returns its pointer
	{
		if (ID[0] == 'u')
		{
			return SearchUserByID(ID);
		}
		else
		{
			return SearchPageByID(ID);
		}
	}
	Post* SearchPostByID(char* ID)		//Searches in userList any post ID that is passed and returns its pointer
	{
		bool found = false;
		Post* postSearched = nullptr;
		for (int i = 0; i < totalPosts; i++)
		{
			if (Helper::CompareString(ID, postList[i]->GetPostID()))
			{
				found = true;
				postSearched = postList[i];
			}
		}
		return postSearched;
	}
	void UpdateFriendList(char*** friendIDs)	//traverses triple pointer to search user IDs and add them to friend List of user through association 
	{
		int j;
		for (int i = 0; i < totalUsers; i++)
		{
			if (friendIDs[i])
			{
				j = 0;
				while (friendIDs[i][j][0] != '-')
				{
					User* friendFound = SearchUserByID(friendIDs[i][j]);
					if (friendFound)
					{
						userList[i]->AddFriend(friendFound);
					}
					j++;
				}
			}
		}
	}
	void LoadUsersFromFile(istream& fin)		//Loading all data of users from file
	{
		fin >> totalUsers;
		userList = new User * [totalUsers];

		char*** friendIDs;
		char delim[] = "-1";
		char buffer[20];
		friendIDs = new char** [totalUsers];

		for (int i = 0; i < totalUsers; i++)
		{
			userList[i] = new User;
			userList[i]->ReadDataFromFile(fin);

			fin >> buffer;
			//reading user friends in double pointer array
			if (!Helper::CompareString(buffer, delim))
			{
				friendIDs[i] = new char* [11];
				int j = 0;
				while (!Helper::CompareString(buffer, delim))
				{
					friendIDs[i][j] = Helper::GetStringFromBuffer(buffer);
					fin >> buffer;
					j++;
				}
				friendIDs[i][j] = Helper::GetStringFromBuffer(delim);
				j++;
				while (j < 11)
				{
					friendIDs[i][j] = nullptr;
					j++;
				}
			}
			else
			{
				friendIDs[i] = nullptr;
			}
			//reading user pages and checking if they exist in pageList and then adding them to user's liked pages through association
			fin >> buffer;
			while (buffer[0] != '-')
			{
				Page* pageFound = SearchPageByID(buffer);
				if (pageFound)
				{
					userList[i]->LikePage(pageFound);
				}
				fin >> buffer;
			}
		}
		UpdateFriendList(friendIDs);
		for (int i = 0; i < totalUsers; i++)
		{
			if (friendIDs[i])
			{
				for (int j = 0; j < 11; j++)
				{
					if (friendIDs[i][j])
						delete[] friendIDs[i][j];
				}
				delete[] friendIDs[i];
			}
		}
		delete[] friendIDs;
	}
	void LoadPagesFromFile(istream& fin)		//Loading all data of pages from file
	{
		fin >> totalPages;
		pageList = new Page * [totalPages];

		for (int i = 0; i < totalPages; i++)
		{
			pageList[i] = new Page;
			pageList[i]->ReadDataFromFile(fin);
		}
	}
	void LoadPosts(istream& fin)			//Loading all data of posts from file
	{
		char buffer[100];
		char c;

		fin >> totalPosts;

		postList = new Post * [totalPosts];

		fin >> c;
		while (c == '-')
		{
			fin.get(c);
		}

		for (int i = 0; i < totalPosts; i++)
		{
			postList[i] = new Post;
			postList[i]->ReadDataFromFile(fin);

			fin >> buffer;
			Object* sharedBy = SearchObjectByID(buffer);

			if (sharedBy)
			{
				postList[i]->SetSharedBy(sharedBy);
				sharedBy->AddPostToTimeline(postList[i]);
			}

			fin >> buffer;
			while (buffer[0] != '-')
			{
				Object* likedBy = SearchObjectByID(buffer);

				if (likedBy)
				{
					postList[i]->SetLikedBy(likedBy);
				}
				fin >> buffer;
			}

			fin >> c;
			while (c == '-')
			{
				fin.get(c);
			}
		}
	}
	void LoadCommentsFromFile(istream& fin)		//Loading all data of comments from file
	{
		char buffer[100];
		char id[30];
		char comment[100];
		int total;

		fin >> total;

		for (int i = 0; i < total; i++)
		{
			Comment* newComment = new Comment;

			fin >> id;
			fin >> buffer;
			Post* commentPost = SearchPostByID(buffer);

			if (commentPost)
				commentPost->AddCommentToList(newComment);

			fin >> buffer;
			Object* commentBy = SearchObjectByID(buffer);

			fin.getline(buffer, 100);

			int j = 0, k = 0;
			bool check = false;
			while (buffer[j] != '\0')
			{
				if (buffer[j] != ' ' || check)
				{
					comment[k] = buffer[j];
					k++;
					check = true;
				}
				j++;
			}
			comment[k] = '\0';

			newComment->SetValues(id, comment, commentBy);
		}
	}
	void Load()		//opens all files needed and calls functions for loading commments, users, pages, and posts
	{
		fstream fin1;
		fstream fin2;
		fstream fin3;
		fstream fin4;

		fin1.open("SocialNetworkPages.txt", ios::in);
		fin2.open("SocialNetworkUsers.txt", ios::in);
		fin3.open("SocialNetworkPosts.txt", ios::in);
		fin4.open("SocialNetworkComments.txt", ios::in);

		if (fin1.is_open() && fin2.is_open() && fin3.is_open() && fin4.is_open())
		{
			LoadPagesFromFile(fin1);
			LoadUsersFromFile(fin2);
			LoadPosts(fin3);
			LoadCommentsFromFile(fin4);
		}
		else
		{
			cout << "Error opening files\n";
		}

		fin1.close();
		fin2.close();
		fin3.close();
		fin4.close();
	}
	void ViewPage(char* pageID)		//prints the data of a page
	{
		Page* pagePtr = SearchPageByID(pageID);
		if (pagePtr)
		{
			pagePtr->ViewSharedPosts();
		}
		else
		{
			cout << "Page not found\n\n";
		}
	}
	void ViewPostLikedList(char* postID)	//prints the IDs and names of all the users and pages that liked a post
	{
		Post* ptr = SearchPostByID(postID);
		if (ptr)
			ptr->ViewLikedList();
		else
			cout << "Post not found\n\n";
	}
	static void Run();
};

void User::LikePost(Post* postPtr)
{
	if (postPtr)
	{
		if (!postPtr->SearchInLiked(this))
			postPtr->SetLikedBy(this);
		else
			cout << "Cannot like post more than once\n\n";
	}
	else
		cout << "Post not found\n\n";
}

void User::PostComment(Post* postPtr, char* text)
{
	if (postPtr)
	{
		Comment* newComment = new Comment;

		newComment->SetValues(text, this);
		postPtr->AddCommentToList(newComment);
	}
}
void User::PrintLikedPageList()
{
	cout << fName << " " << lName << " - Liked Pages\n\n";
	for (int i = 0; i < likedPagesCount; i++)
	{
		likedPages[i]->PrintListView();
	}
	cout << endl;
}

void User::ViewTimeline()
{
	PrintName();
	cout << " - " << "Timeline\n\n";
	for (int i = 0; i < timelineCount; i++)
	{
		Timeline[i]->PrintPost();
		cout << endl;
	}
}

void Post::PrintPost()
{
	cout << "---";
	SharedBy->PrintName();
	if (activity)
	{
		activity->Print();
		cout << "   \"" << text << "\"" << " ...";
	}
	else
	{
		cout << " shared ";
		cout << "\"" << text << "\"" << " ...";
	}
	
	SharedDate.Print();
	cout << endl;

	for (int i = 0; i < commentCount; i++)
	{
		cout << "\t\t";
		commentList[i]->Print();
	}
}

void Post::PrintPostWithoutDate()
{
	cout << "---";
	SharedBy->PrintName();
	if (activity)
	{
		activity->Print();
		cout << "   \"" << text << "\"";
	}
	else
	{
		cout << " shared ";
		cout << "\"" << text << "\"";
	}

	cout << endl;

	for (int i = 0; i < commentCount; i++)
	{
		cout << "\t\t";
		commentList[i]->Print();
	}
}

void Post::PrintPostWithoutComment()
{
	cout << "---";
	SharedBy->PrintName();
	if (activity)
	{
		activity->Print();
		cout << "   \"" << text << "\"" << " ...";
	}
	else
	{
		cout << " shared ";
		cout << "\"" << text << "\"" << " ...";
	}

	SharedDate.Print();
	cout << endl;
}

void User::ViewSharedPostsByCurrentDate()
{
	for (int i = 0; i < timelineCount; i++)
	{
		if (Timeline[i]->CompareDate())
		{
			Timeline[i]->PrintPostWithoutDate();
			cout << endl;
		}
	}
}

void Page::ViewSharedPostsByCurrentDate()
{
	for (int i = 0; i < timelineCount; i++)
	{
		if (Timeline[i]->CompareDate())
		{
			Timeline[i]->PrintPostWithoutDate();
			cout << endl;
		}
	}
}

void Page::ViewSharedPosts()
{
	PrintName();
	cout << endl;
	for (int i = 0; i < timelineCount; i++)
	{
		Timeline[i]->PrintPost();
		cout << endl;
	}
}

void User::ViewHome()
{
	PrintName();
	cout << " - Homepage\n\n";

	for (int i = 0; i < friendCount; i++)
	{
		friendsList[i]->ViewSharedPostsByCurrentDate();
	}

	for (int i = 0; i < likedPagesCount; i++)
	{
		likedPages[i]->ViewSharedPostsByCurrentDate();
	}
}

void User::SeeYourMemories()
{
	int yearDiff;
	int check = 1;
	int memoriesExist = 0;

	for (int i = 0; i < timelineCount; i++)
	{
		yearDiff = Timeline[i]->CalculateYearDifference();
		if (yearDiff != -1)
		{
			if (check)
				cout << "We hope you enjoy looking back and sharing your memories on Facebook, from the most recent to those long ago.\n\n";
			cout << "On this day\n" << yearDiff << " years ago\n";
			Timeline[i]->PrintPost();
			cout << endl;
			check = 0;
			memoriesExist = 1;
		}
	}
	if (!memoriesExist)
		cout << "You have no memories.\n";
}

void User::ShareMemory(Post* postPtr, char* memoryText)
{
	if (Timeline)
	{
		if (postPtr->CalculateYearDifference() != -1)
		{
			Timeline[timelineCount] = new Memory(postPtr, memoryText);
			timelineCount++;
		}
	}
}

void SocialMedia::Run()
{
	SocialMedia fb;
	cout << "Command:\tSet current System Date 15 11 2017\n";
	Date::SetCurrentDate();
	Date::PrintCurrentDate();

	fb.Load();

	User* currUser = nullptr;
	char userID[] = "u7";
	char postID[] = "post5";
	char post4ID[] = "post4";
	char post8ID[] = "post8";
	char post10ID[] = "post10";
	char pageID[] = "p1";
	char commentText[] = "Above comment already existed so I added another";
	char commentText2[] = "Thanks for the wishes";
	char memoryText[] = "Never thought I will be specialist in this field...";

	cout << "\nCommand:\tSet current user u7\n";
	currUser = fb.SearchUserByID(userID);
	if (currUser)
	{
		currUser->PrintName();
		cout << " successfully set as current user\n\n";

		cout << "Command:\tView Friend List";
		cout << "\n---------------------------------------------------------------------------------------------------------------\n";
		currUser->PrintFriendsList();
		cout << "\n---------------------------------------------------------------------------------------------------------------\n";

		cout << "Command:\tView Liked Pages";
		cout << "\n---------------------------------------------------------------------------------------------------------------\n";
		currUser->PrintLikedPageList();
		cout << "\n---------------------------------------------------------------------------------------------------------------\n";

		cout << "Command:\tView Home";
		cout << "\n---------------------------------------------------------------------------------------------------------------\n";
		currUser->ViewHome();
		cout << "\n---------------------------------------------------------------------------------------------------------------\n";

		cout << "Command:\tView Timeline";
		cout << "\n---------------------------------------------------------------------------------------------------------------\n";
		currUser->ViewTimeline();
		cout << "\n---------------------------------------------------------------------------------------------------------------\n";

		cout << "Command:\tViewLikedList(post5)\n\n";
		fb.ViewPostLikedList(postID);
		cout << "\n---------------------------------------------------------------------------------------------------------------\n";

		cout << "Command:\tLikePost(post5)\n";
		cout << "Command:\tViewLikedList(post5)\n\n";
		Post* postPtr = fb.SearchPostByID(postID);
		if (postPtr)
			currUser->LikePost(postPtr);
		else
			cout << "Post not found\n\n";
		fb.ViewPostLikedList(postID);

		cout << "\n---------------------------------------------------------------------------------------------------------------\n";
		cout << "Command:\tPostComment(post4, \"Above comment already existed so I added another\")\n";
		cout << "Command:\tViewPost(post4)\n\n";
		postPtr = fb.SearchPostByID(post4ID);
		if (postPtr)
		{
			currUser->PostComment(postPtr, commentText);
			postPtr->PrintPost();
			cout << endl;
		}
		else
			cout << "Post not found\n\n";
		cout << "\n---------------------------------------------------------------------------------------------------------------\n";
		cout << "Command:\tPostComment(post8, \"Thanks for the wishes\")\n";
		cout << "Command:\tViewPost(post8)\n\n";
		postPtr = fb.SearchPostByID(post8ID);
		if (postPtr)
		{
			currUser->PostComment(postPtr, commentText2);
			postPtr->PrintPost();
			cout << endl;
		}
		else
			cout << "Post not found\n\n";

		cout << "\n---------------------------------------------------------------------------------------------------------------\n";
		cout << "Command:\tSeeYourMemories()\n\n";
		currUser->SeeYourMemories();

		cout << "\n---------------------------------------------------------------------------------------------------------------\n";
		cout << "Command:\tShareMemory(post10, \"Never thought I will be specialist in this field...)\"\n";
		cout << "Command:\tView Timline\n\n";
		postPtr = fb.SearchPostByID(post10ID);
		if (postPtr)
			currUser->ShareMemory(postPtr, memoryText);
		else
			cout << "Post not found\n\n";
		currUser->ViewTimeline();

	}
	else
	{
		cout << "User not found\n\n";
	}

	cout << "\n---------------------------------------------------------------------------------------------------------------\n";
	cout << "Command:\tViewPage(p1)\n\n";
	fb.ViewPage(pageID);

	cout << "\n---------------------------------------------------------------------------------------------------------------\n";
	cout << "Command:\tSet current user \"u11\"\n";
	char newUserID[] = "u11";
	currUser = fb.SearchUserByID(newUserID);
	if (currUser)
	{
		cout << "Ahsan Ali successfully set as Current User\n\n";
		currUser->ViewHome();
		cout << "\n---------------------------------------------------------------------------------------------------------------\n";
		currUser->ViewTimeline();
	}
	else
	{
		cout << "User not found\n\n";
	}
}

int main()
{
	SocialMedia::Run();
}