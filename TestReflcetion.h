#pragma once
#include <QObject>

class TestReflcetion :
    public QObject
{
    Q_OBJECT;
private slots:
    void test_number_str();
    void test_vec();
    void test_nested();


};

