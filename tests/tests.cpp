#include <gtest/gtest.h>
#include "Student.h"
#include "JuniorStudent.h"
#include "SeniorStudent.h"
#include "GraduateStudent.h"
#include "HashTable.h"
#include <memory>
#include <vector>
#include <string>

using namespace university;

// --- Student Tests ---

TEST(JuniorStudentTest, ConstructorAndGetters) {
    std::vector<int> grades = {5, 4, 5};
    JuniorStudent student("Ivanov I.I.", "IU7-21B", 101, grades);

    EXPECT_EQ(student.getCategory(), StudentCategory::JUNIOR);
    EXPECT_EQ(student.getSessionGrades(), grades);
}

TEST(JuniorStudentTest, ThrowsOnTooManyGrades) {
    std::vector<int> tooManyGrades = {5, 4, 5, 3, 4, 5};
    EXPECT_THROW(
        JuniorStudent("Petrov P.P.", "IU7-22B", 102, tooManyGrades),
        std::invalid_argument
    );

    JuniorStudent student("Sidorov S.S.", "IU7-23B", 103, {5, 5});
    EXPECT_THROW(
        student.setSessionGrades(tooManyGrades),
        std::invalid_argument
    );
}

TEST(SeniorStudentTest, ConstructorAndGetters) {
    std::vector<int> grades = {5, 4, 5, 4};
    ResearchWork work = {"AI in medicine", "Lab 101", 5, 5};
    SeniorStudent student("Smirnov A.A.", "IU6-41M", 104, grades, work);

    EXPECT_EQ(student.getCategory(), StudentCategory::SENIOR);
    EXPECT_EQ(student.getSessionGrades(), grades);
    EXPECT_EQ(student.getResearchWork().topic, "AI in medicine");
}

TEST(GraduateStudentTest, ConstructorAndGetters) {
    DiplomaProject project = {"Blockchain voting", "Tech Park", 5, 5, 5};
    GraduateStudent student("Kuznetsov K.K.", "IU5-81M", 105, project);

    EXPECT_EQ(student.getCategory(), StudentCategory::GRADUATE);
    EXPECT_EQ(student.getDiplomaProject().topic, "Blockchain voting");
}

// --- HashTable Tests ---

TEST(HashTableTest, InsertAndFind) {
    HashTable<int, std::string> table;
    ASSERT_TRUE(table.insert(1, "one"));
    ASSERT_TRUE(table.insert(2, "two"));
    
    auto found = table.find(1);
    ASSERT_TRUE(found.has_value());
    EXPECT_EQ(found->get(), "one");

    auto notFound = table.find(3);
    EXPECT_FALSE(notFound.has_value());
}

TEST(HashTableTest, InsertDuplicate) {
    HashTable<int, std::string> table;
    ASSERT_TRUE(table.insert(1, "one"));
    ASSERT_FALSE(table.insert(1, "another_one"));
    EXPECT_EQ(table.size(), 1);
}

TEST(HashTableTest, Remove) {
    HashTable<int, std::string> table;
    table.insert(1, "one");
    table.insert(2, "two");

    ASSERT_TRUE(table.remove(1));
    EXPECT_EQ(table.size(), 1);
    EXPECT_FALSE(table.find(1).has_value());

    ASSERT_FALSE(table.remove(3)); // Not in table
    EXPECT_EQ(table.size(), 1);
}

TEST(HashTableTest, Rehash) {
    HashTable<int, int> table(4); // Small initial size
    for (int i = 0; i < 10; ++i) {
        ASSERT_TRUE(table.insert(i, i * 10));
    }
    EXPECT_EQ(table.size(), 10);

    for (int i = 0; i < 10; ++i) {
        auto found = table.find(i);
        ASSERT_TRUE(found.has_value());
        EXPECT_EQ(found->get(), i * 10);
    }
}

TEST(HashTableTest, Iterator) {
    HashTable<int, std::string> table;
    table.insert(1, "one");
    table.insert(10, "ten");
    table.insert(20, "twenty");

    int count = 0;
    std::map<int, std::string> expected = {{1, "one"}, {10, "ten"}, {20, "twenty"}};
    for (auto it = table.begin(); it != table.end(); ++it) {
        auto& pair = *it;
        EXPECT_EQ(expected.count(pair.first), 1);
        EXPECT_EQ(expected[pair.first], pair.second);
        count++;
    }
    EXPECT_EQ(count, 3);
}

TEST(HashTableTest, StudentPolymorphism) {
    HashTable<int, std::unique_ptr<Student>> studentTable;

    auto junior = std::make_unique<JuniorStudent>("Junior", "J1", 1, std::vector<int>{5});
    auto senior = std::make_unique<SeniorStudent>("Senior", "S1", 2, std::vector<int>{4}, ResearchWork{"Topic", "Place", 5, 5});

    studentTable.insert(101, std::move(junior));
    studentTable.insert(202, std::move(senior));

    auto foundJunior = studentTable.find(101);
    ASSERT_TRUE(foundJunior.has_value());
    EXPECT_EQ((*foundJunior)->get()->getCategory(), StudentCategory::JUNIOR);

    auto foundSenior = studentTable.find(202);
    ASSERT_TRUE(foundSenior.has_value());
    EXPECT_EQ((*foundSenior)->get()->getCategory(), StudentCategory::SENIOR);
} 