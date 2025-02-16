#include "TestReflcetion.h"
#include <QTest>
#include "Serializer.h"
#include "Types.h"


using namespace struct_serialize;


void TestReflcetion::test_number_str()
{
    int a = 1;
    double b = 2.0;
	Point var{a,b,true,"helloC++","helloQt"};
	bool ret = false;
	QJsonObject json;
	//ret = Serializ(var, json);
	ret = var.serialize(json);
	//qDebug().noquote() << str;
	QVERIFY(ret == true);

	Point par;
	//ret = Deserializ(json,par);
	ret = par.deserialize(json);
	QVERIFY(ret == true);
}

void TestReflcetion::test_vec()
{
	std::vector<int> x{1,2,3};
	std::vector<double> y{0.9,0.8,0.7};
	std::vector<std::string> z{"aaa","bbb"};
	std::vector<QString> q{"xx","yy"};

	ContainerNest var{q,x,y,z,{q,q} };
	bool ret = false;
	QJsonObject json;
	//ret = Serializ(var, json);
	ret = var.serialize(json);
	//qDebug().noquote() << str;
	QVERIFY(ret == true);

	ContainerNest par;
	//ret = Deserializ(json, par);
	ret = par.deserialize(json);
	QVERIFY(ret == true);
	
}

void TestReflcetion::test_nested()
{
    int a = 1;
    double b = 2.0;
	Point pt{ a,b,true,"helloC++","helloQt" };

	std::vector<int> x{ 1,2,3 };
	std::vector<double> y{ 0.9,0.8,0.7 };
	std::vector<std::string> z{ "aaa","bbb" };
	std::vector<QString> q{ "xx","yy" };
	ContainerNest cn{ q,x,y,z,{q,q} };

	Nested var{pt,cn,"my name is 1","my name is 2",998};
	bool ret = false;
	QString str;
	//ret = Serializ(var, str);
	ret = var.serialize(str);
	//qDebug().noquote() << str;
	QVERIFY(ret == true);

	QString str1;
	QJsonObject obj;
	//ret = Serializ(var, obj);
	ret = var.serialize(obj);
	QVERIFY(ret == true);
	Nested par;
	//ret = Deserializ(obj,par);
	//ret = Deserializ(obj,par);
	ret = par.deserialize(obj);
	QVERIFY(ret == true);
	//ret = Serializ(par, str1);
	ret = par.serialize(str1);
	QVERIFY(ret == true);

	QVERIFY(str == str1);
}


