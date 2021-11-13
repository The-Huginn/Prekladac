// LexicalAnalyzerTests.cpp      

extern "C" 
{
    #include "../../src/Utils/list.h"
    #include "../../src/Utils/stack.h"
    #include "../../src/Utils/vector.h"
    #include "../../src/Utils/vector_testonly.h"
    #include "../../src/Utils/symtable_testonly.h"
    #include "../../src/Utils/logger.h"
}

#include <gtest/gtest.h>
#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>

typedef struct MyVec2
{
    int x, y;
} Vec2;

Vec2* Vec2_Init(int x, int y)
{
    Vec2* vec = (Vec2*)malloc(sizeof(Vec2));
    if (vec == NULL)
        ERROR("Allocation failed!");
    vec->x = x;
    vec->y = y;
    return vec;
}

void Vec2_Destroy(Vec2* vec)
{
    free(vec);
}

typedef struct MyVec3
{
    int x, y, z;
} Vec3;

Vec3* Vec3_Init(int x, int y, int z)
{
    Vec3* vec = (Vec3*)malloc(sizeof(Vec3));
    if (vec == NULL)
        ERROR("Allocation failed!");
    vec->x = x;
    vec->y = y;
    vec->z = z;
    return vec;
}

void Vec3_Destroy(Vec3* vec)
{
    free(vec);
}

bool Vec2Vec3_Comp(Vec2* vec2, Vec3* vec3)
{
    return (vec2->x == vec3->x) && (vec2->y == vec3->y) && (vec3->z == 0);
}

class StackTests : public ::testing::Test
{
    public:
    Stack* vec2;

    void SetUp() override
    {
        vec2 = Stack_Init((void (*)(void*))Vec2_Destroy);
    }

    void TearDown() override
    {
        Stack_Destroy(vec2);
    }
};


TEST_F(StackTests, 1_Init) {
    ASSERT_NE(vec2, nullptr);
    EXPECT_EQ(Stack_Top(vec2), nullptr);
}

TEST_F(StackTests, 2_PushPop){
    Stack_Push(vec2, Vec2_Init(3, 7));
    Vec2* top = (Vec2*)Stack_Top(vec2);
    EXPECT_EQ(top->x, 3);
    EXPECT_EQ(top->y, 7);

    Stack_Push(vec2, Vec2_Init(5, 0));
    top = (Vec2*)Stack_Top(vec2);
    EXPECT_EQ(top->x, 5);
    EXPECT_EQ(top->y, 0);

    Stack_Push(vec2, Vec2_Init(14, -9));
    top = (Vec2*)Stack_Top(vec2);
    EXPECT_EQ(top->x, 14);
    EXPECT_EQ(top->y, -9);

    Stack_Pop(vec2);
    top = (Vec2*)Stack_Top(vec2);
    EXPECT_EQ(top->x, 5);
    EXPECT_EQ(top->y, 0);

    Stack_Pop(vec2);
    top = (Vec2*)Stack_Top(vec2);
    EXPECT_EQ(top->x, 3);
    EXPECT_EQ(top->y, 7);

    Stack_Pop(vec2);
    top = (Vec2*)Stack_Top(vec2);
    EXPECT_EQ(top, nullptr);
}

TEST_F(StackTests, 3_Clear)
{
    Stack_Push(vec2, Vec2_Init(3, 7));
    Stack_Push(vec2, Vec2_Init(5, 0));
    Stack_Push(vec2, Vec2_Init(14, -9));
   
    Stack_Clear(vec2);
    Vec2* top = (Vec2*)Stack_Top(vec2);
    EXPECT_EQ(top, nullptr);
}

class ListTests : public ::testing::Test
{
    public:
    LList* vec2;

    void SetUp() override
    {
        vec2 = List_Init((void (*)(void*))Vec2_Destroy, (const bool (*)(void*, const void*))Vec2Vec3_Comp);
    }

    void TearDown() override
    {
        List_Destroy(vec2);
    }
};

TEST_F(ListTests, 1_Init)
{
    ASSERT_NE(vec2, nullptr);
    EXPECT_EQ(vec2->begin, nullptr);
}

TEST_F(ListTests, 2_Add)
{
    List_AddFirst(vec2, Vec2_Init(3, 7));

    Vec3* vec3 = Vec3_Init(3, 7, 0);
    Vec2* search = (Vec2*)List_GetData(vec2, vec3);
    EXPECT_EQ(search->x, vec3->x);
    EXPECT_EQ(search->y, vec3->y);

    List_AddFirst(vec2, Vec2_Init(5, 0));
    Vec3_Destroy(vec3);
    vec3 = Vec3_Init(5, 0, 1);
    search = (Vec2*)List_GetData(vec2, vec3);
    EXPECT_EQ(search, nullptr);

    Vec3_Destroy(vec3);
    vec3 = Vec3_Init(5, 0, 0);
    search = (Vec2*)List_GetData(vec2, vec3);
    EXPECT_EQ(search->x, vec3->x);
    EXPECT_EQ(search->y, vec3->y);

    List_RemoveFirst(vec2);
    search = (Vec2*)List_GetData(vec2, vec3);
    EXPECT_EQ(search, nullptr);
}

TEST_F(ListTests, 3_Clear)
{
    List_AddFirst(vec2, Vec2_Init(3, 7));
    List_AddFirst(vec2, Vec2_Init(5, 0));
    List_AddFirst(vec2, Vec2_Init(14, -9));
    
    Vec3* vec3 = Vec3_Init(3, 7, 0);
    Vec2* search = (Vec2*)List_GetData(vec2, vec3);
    EXPECT_EQ(search->x, vec3->x);
    EXPECT_EQ(search->y, vec3->y);

    List_Clear(vec2);

    search = (Vec2*)List_GetData(vec2, vec3);
    EXPECT_EQ(search, nullptr);
}

typedef struct
{
    Symtable *symtable;
    LList *buffer;
    Stack *scopes;
    HTable *table;
} SymbolTable_t;

SymbolTable_t *SymbolTable_Init()
{
    SymbolTable_t *symtable = (SymbolTable_t*) malloc(sizeof(SymbolTable_t));
    if (symtable == NULL)
        ERROR("Allocation failed!");

    symtable->symtable = Symtable_Init();
    symtable->buffer = NULL;
    symtable->scopes = NULL;
    symtable->table = NULL;

    return symtable;
}

void SymbolTable_Destroy(SymbolTable_t *symtable)
{
    Symtable_Destroy(symtable->symtable);
    free(symtable);
}

void SymbolTable_Update(SymbolTable_t *symtable)
{
    symtable->buffer = Symtable_GetBufferList(symtable->symtable);
    symtable->scopes = Symtable_GetScopeStack(symtable->symtable);
    symtable->table = Symtable_GetHashtable(symtable->symtable);
}

class SymbolTableTests : public ::testing::Test
{
    public:
    SymbolTable_t *symtable;

    void SetUp() override
    {
        symtable = SymbolTable_Init();
        SymbolTable_Update(symtable);
    }

    void TearDown() override
    {
        SymbolTable_Destroy(symtable);
    }
};

TEST_F(SymbolTableTests, 1_Init)
{
    EXPECT_EQ(List_GetFirst(symtable->buffer), nullptr);
    EXPECT_FALSE(symtable->table == nullptr);
    EXPECT_FALSE(Stack_Top(symtable->scopes) == nullptr);
}

TEST_F(SymbolTableTests, 2_Add)
{
    Element *element = Symtable_CreateElement(symtable->symtable, "ID1", 10);

    EXPECT_FALSE(element == nullptr);

    Element *saved_element = Symtable_GetElement(symtable->symtable, "ID1");

    EXPECT_EQ(element, saved_element);
}

TEST_F(SymbolTableTests, 3_ScopeFunctions1)
{
    Symtable_AddScope(symtable->symtable);

    Element *element = Symtable_CreateElement(symtable->symtable, "ID1", 10);

    Symtable_PopScopeToBuffer(symtable->symtable);

    EXPECT_FALSE(element == nullptr);
    EXPECT_EQ(Symtable_GetElement(symtable->symtable, "ID1"), nullptr);
    EXPECT_EQ(Symtable_GetElementFromBuffer(symtable->symtable, "ID1"), element);
}

TEST_F(SymbolTableTests, 4_MultipleScopesWithSameName)
{
    Element *global = Symtable_CreateElement(symtable->symtable, "global", 1);
    Symtable_AddScope(symtable->symtable);
    Element *local_1_1 = Symtable_CreateElement(symtable->symtable, "local", 1);
    Symtable_AddScope(symtable->symtable);
    Element *local_2_1 = Symtable_CreateElement(symtable->symtable, "local", 1);

    EXPECT_EQ(local_2_1, Symtable_GetElement(symtable->symtable, "local"));

    Symtable_PopScope(symtable->symtable);

    EXPECT_EQ(local_1_1, Symtable_GetElement(symtable->symtable, "local"));
}

TEST_F(SymbolTableTests, 5_MultipleScopesWithSameNamePoping)
{
    Element *global = Symtable_CreateElement(symtable->symtable, "global", 1);
    Symtable_AddScope(symtable->symtable);
    Element *local_1_1 = Symtable_CreateElement(symtable->symtable, "local", 1);
    Symtable_AddScope(symtable->symtable);
    Element *local_2_1 = Symtable_CreateElement(symtable->symtable, "local", 1);

    EXPECT_EQ(local_2_1, Symtable_GetElement(symtable->symtable, "local"));

    Symtable_PopScope(symtable->symtable);

    EXPECT_EQ(local_1_1, Symtable_GetElement(symtable->symtable, "local"));

    Symtable_AddScope(symtable->symtable);
    local_2_1 = Symtable_CreateElement(symtable->symtable, "local", 1);

    EXPECT_EQ(local_2_1, Symtable_GetElement(symtable->symtable, "local"));

    Symtable_PopScope(symtable->symtable);
    EXPECT_EQ(local_1_1, Symtable_GetElement(symtable->symtable, "local"));
}

TEST_F(SymbolTableTests, 6_MultipleVarsInScope)
{
    Symtable_AddScope(symtable->symtable);
    Element *local1 = Symtable_CreateElement(symtable->symtable, "local", 1);
    Element *local2 = Symtable_CreateElement(symtable->symtable, "local", 1);

    EXPECT_TRUE(local1 == Symtable_GetElement(symtable->symtable, "local") || local2 == Symtable_GetElement(symtable->symtable, "local"));
}

TEST_F(SymbolTableTests, 7_FindingNonExisting)
{
    EXPECT_EQ(nullptr, Symtable_GetElement(symtable->symtable, "local"));
    EXPECT_EQ(nullptr, Symtable_GetElementFromBuffer(symtable->symtable, "local"));
}

TEST_F(SymbolTableTests, 8_FindingOlderScopes)
{
    Element *global = Symtable_CreateElement(symtable->symtable, "global", 1);
    Symtable_AddScope(symtable->symtable);
    Element *local_1_1 = Symtable_CreateElement(symtable->symtable, "local", 1);
    Symtable_AddScope(symtable->symtable);

    EXPECT_EQ(global, Symtable_GetElement(symtable->symtable, "global"));

    Element *local_2_1 = Symtable_CreateElement(symtable->symtable, "new local", 1);

    EXPECT_EQ(local_1_1, Symtable_GetElement(symtable->symtable, "local"));
}

TEST_F(SymbolTableTests, 9_AdvancedCombinations1)
{
    Element *global = Symtable_CreateElement(symtable->symtable, "global", 1);
    Symtable_AddScope(symtable->symtable);
    Element *local_1_1 = Symtable_CreateElement(symtable->symtable, "local", 1);
    Symtable_AddScope(symtable->symtable);
    Element *local_2_1 = Symtable_CreateElement(symtable->symtable, "local", 1);

    EXPECT_EQ(local_2_1, Symtable_GetElement(symtable->symtable, "local"));

    Symtable_PopScope(symtable->symtable);

    EXPECT_EQ(local_1_1, Symtable_GetElement(symtable->symtable, "local"));

    Symtable_AddScope(symtable->symtable);
    local_2_1 = Symtable_CreateElement(symtable->symtable, "local", 1);

    EXPECT_EQ(local_2_1, Symtable_GetElement(symtable->symtable, "local"));

    Symtable_PopScope(symtable->symtable);
    EXPECT_EQ(local_1_1, Symtable_GetElement(symtable->symtable, "local"));

    Symtable_PopScopeToBuffer(symtable->symtable);
    EXPECT_EQ(global, Symtable_GetElement(symtable->symtable, "global"));

    EXPECT_EQ(local_1_1, Symtable_GetElementFromBuffer(symtable->symtable, "local"));
}

TEST_F(SymbolTableTests, 10_AdvancedCombinations2)
{
    Symtable_AddScope(symtable->symtable);
    Element *local_1 = Symtable_CreateElement(symtable->symtable, "local1", 1);

    Symtable_PopScopeToBuffer(symtable->symtable);
    Symtable_AddScope(symtable->symtable);

    Element *local_2 = Symtable_CreateElement(symtable->symtable, "local2", 1);

    Symtable_PopScopeToBuffer(symtable->symtable);

    EXPECT_EQ(local_1, Symtable_GetElementFromBuffer(symtable->symtable, "local1"));
    EXPECT_EQ(local_2, Symtable_GetElementFromBuffer(symtable->symtable, "local2"));
}

TEST_F(SymbolTableTests, 11_AdvancedCombinations3)
{
    Symtable_AddScope(symtable->symtable);
    Element *local_1 = Symtable_CreateElement(symtable->symtable, "local1", 1);

    Symtable_AddScope(symtable->symtable);

    Element *local_2 = Symtable_CreateElement(symtable->symtable, "local2", 1);

    Symtable_PopScopeToBuffer(symtable->symtable);
    Symtable_PopScopeToBuffer(symtable->symtable);

    EXPECT_EQ(local_1, Symtable_GetElementFromBuffer(symtable->symtable, "local1"));
    EXPECT_EQ(local_2, Symtable_GetElementFromBuffer(symtable->symtable, "local2"));
}

TEST_F(SymbolTableTests, 12_AdvancedCombinations4)
{
    Symtable_AddScope(symtable->symtable);

    Element *element = Symtable_CreateElement(symtable->symtable, "local", 1);

    Symtable_PopScope(symtable->symtable);

    EXPECT_EQ(nullptr, Symtable_GetElement(symtable->symtable, "local"));

    element = Symtable_CreateElement(symtable->symtable, "local", 1);

    EXPECT_EQ(element, Symtable_GetElement(symtable->symtable, "local"));
}

TEST_F(SymbolTableTests, 13_ClearingBuffer)
{
    Symtable_AddScope(symtable->symtable);

    Element *element = Symtable_CreateElement(symtable->symtable, "local", 1);

    Symtable_PopScopeToBuffer(symtable->symtable);
    Symtable_ClearBuffer(symtable->symtable);

    element = Symtable_CreateElement(symtable->symtable, "local", 1);

    Symtable_PopScopeToBuffer(symtable->symtable);

    EXPECT_EQ(element, Symtable_GetElementFromBuffer(symtable->symtable, "local"));

    Symtable_AddScope(symtable->symtable);
    element = Symtable_CreateElement(symtable->symtable, "local", 1);
    Symtable_ClearBuffer(symtable->symtable);

    EXPECT_EQ(element, Symtable_GetElement(symtable->symtable, "local"));

    Symtable_PopScopeToBuffer(symtable->symtable);

    EXPECT_EQ(element, Symtable_GetElementFromBuffer(symtable->symtable, "local"));

    Symtable_ClearBuffer(symtable->symtable);

    EXPECT_EQ(nullptr, Symtable_GetElement(symtable->symtable, "local"));
    EXPECT_EQ(nullptr, Symtable_GetElementFromBuffer(symtable->symtable, "local"));
}

void FreeElement(int* data)
{
    free(data);
}

class VectorTests : public ::testing::Test
{
    public:
    Vector *vector;

    int* CreateElement(int number)
    {
        int *a = (int*)malloc(sizeof(int));
        *a = number;
        return a;
    }

    void PushBackVector()
    {
        for (int i = 0; i < 10; i++)
            Vector_PushBack(vector, CreateElement(i));
    }

    void InsertVector()
    {
        for (int i = 0; i < 5; i++)
            Vector_InsertElement(vector, i, CreateElement(i));
    }

    void SetUp() override
    {
        vector = Vector_Init((void (*)(void*))FreeElement);
    }

    void TearDown() override
    {
        Vector_Destroy(vector);
    }
};

TEST_F(VectorTests, 1_PushValue)
{
    EXPECT_EQ(Vector_IsEmpty(vector), true);
    EXPECT_NO_THROW(Vector_PushBack(vector, CreateElement(1)));
    EXPECT_EQ(*((int*)Vector_Back(vector)), 1);
}

TEST_F(VectorTests, 2_InsertValue)
{
    Vector_InsertElement(vector, 0, CreateElement(1));
    EXPECT_EQ(*((int*)Vector_GetElement(vector, 0)), 1);
}

TEST_F(VectorTests, 3_PushMultiplePushValues)
{
    PushBackVector();
    EXPECT_EQ(Vector_Size(vector), 10);
    for (int i = 0; i < Vector_Size(vector); i++)
        EXPECT_EQ(*((int*)Vector_GetElement(vector, i)), i);
}

TEST_F(VectorTests, 4_MultipleInsertValues)
{
    InsertVector();
    EXPECT_EQ(Vector_Size(vector), 5);
    for (int i = 0; i < Vector_Size(vector); i++)
        EXPECT_EQ(*((int*)Vector_GetElement(vector, i)), i);
}

TEST_F(VectorTests, 5_PopBackValue)
{
    PushBackVector();
    Vector_PopBack(vector);
    EXPECT_EQ(Vector_Size(vector), 9);
    for (int i = 0; i < Vector_Size(vector); i++)
        EXPECT_EQ(*((int*)Vector_GetElement(vector, i)), i);
}

TEST_F(VectorTests, 5_RemoveLastValue)
{
    PushBackVector();
    Vector_RemoveElement(vector, 9);
    EXPECT_EQ(Vector_Size(vector), 9);
    for (int i = 0; i < Vector_Size(vector); i++)
        EXPECT_EQ(*((int*)Vector_GetElement(vector, i)), i);
}

TEST_F(VectorTests, 6_PopBackAllValues)
{
    PushBackVector();
    for (int i = 0; i < 10; i++)
    {
        Vector_PopBack(vector);
        EXPECT_EQ(Vector_Size(vector), 10-i-1);
        for (int j = 0; j < Vector_Size(vector); j++)
            EXPECT_EQ(*((int*)Vector_GetElement(vector, j)), j);
    }
}

TEST_F(VectorTests, 7_RemoveAllValues)
{
    InsertVector();
    for (int i = 0; i < 5; i++)
    {
        Vector_RemoveElement(vector, 5-i-1);
        EXPECT_EQ(Vector_Size(vector), 5-i-1);
        if (i != 4) 
            EXPECT_EQ(*((int*)Vector_Back(vector)), 5-i-2);
        else
            EXPECT_EQ(((int*)Vector_Back(vector)), nullptr);
    }
}

TEST_F(VectorTests, 8_ClearAndPushBack)
{
    InsertVector();
    Vector_Clear(vector);
    EXPECT_EQ(Vector_Size(vector), 0);
    PushBackVector();
    for (int i = 0; i < 10; i ++)
    {
        EXPECT_EQ(*((int*)Vector_GetElement(vector, i)), i);
    }
}

TEST_F(VectorTests, 9_ClearAndPushBack)
{
    PushBackVector();
    EXPECT_EQ(Vector_IsEmpty(vector), false);
    Vector_Clear(vector);
    EXPECT_EQ(Vector_IsEmpty(vector), true);
    InsertVector();
    for (int i = 4; i >= 0; i--)
    {
        EXPECT_EQ(*((int*)Vector_Back(vector)), i);
        Vector_PopBack(vector);
    }
}

TEST_F(VectorTests, 10_DestroyAndInit)
{
    PushBackVector();
    TearDown();
    SetUp();
    EXPECT_EQ(Vector_Size(vector), 0);
    EXPECT_EQ(Vector_IsEmpty(vector), true);
    InsertVector();
    EXPECT_EQ(Vector_Size(vector), 5);
    EXPECT_EQ(Vector_IsEmpty(vector), false);
    for (int i = 4; !Vector_IsEmpty(vector); i--)
    {
        EXPECT_EQ(*((int*)Vector_Back(vector)), i);
        Vector_PopBack(vector);
    }
}

TEST_F(VectorTests, 11_LargeVector)
{
    for (int i = 0; i < 10; i++)
        PushBackVector();

    for (int i = 0; i < 100; i++)
        EXPECT_EQ(*((int*)Vector_GetElement(vector, i)), i % 10);

    for (int i = 0; i < 87; i++)
    {
        Vector_PopBack(vector);
    }

    EXPECT_EQ(Vector_Size(vector), 13);
    for (int i = 0; i < Vector_Size(vector); i++)
    {
        EXPECT_EQ(*((int*)Vector_GetElement(vector, i)), i % 10);
    }
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    /*if (argc == 1)
    {
        ::testing::GTEST_FLAG(filter) = "";
    }
    if (argc == 2)
    {
        std::string tests = "Test1.1*";
        tests += ":Test1.2*";
        ::testing::GTEST_FLAG(filter) = tests;
    }*/
    return RUN_ALL_TESTS();
}

