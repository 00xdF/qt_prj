#include<qstring.h>
//User¶ÔÏó
class User {
private:
	int id;
	QString username;
public:
	User(int id,QString username) {
		this->id = id;
		this->username = username;
	}

	~User() {

	}

	int getId() {
		return this->id;
	}

	QString getUsername() {
		return this->username;
	}

	QString toString() {
		return "id="+QString::number(id) + ",username=" + username;
	}

};