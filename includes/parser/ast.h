#ifndef __AST_H
#define __AST_H

/**
 * This contains all of the nodes, enums, etc
 * related to the AST. It's all in a separate file
 * because if you haven't noticed, there are fucking
 * loads.
 *
 * The node types represent every type of node that there
 * is in the language, including macros.
 */

#include "util.h"
#include "vector.h"

/** forward declarations */
typedef struct s_Expression Expression;
typedef struct s_Type Type;

typedef struct { char a; } ContinueStat;

typedef struct { char a; } BreakStat;

/** different types of node for a cleanup */
typedef enum {
	IDENTIFIER_LIST_NODE, IDENTIFIER_NODE, LITERAL_NODE, BINARY_EXPR_NODE,
	PRIMARY_EXPR_NODE, EXPR_NODE, TYPE_NAME_NODE, TYPE_LIT_NODE, PAREN_EXPR_NODE,
	ARRAY_TYPE_NODE, POINTER_TYPE_NODE, FIELD_DECL_NODE, UNARY_EXPR_NODE,
	FIELD_DECL_LIST_NODE, STRUCT_DECL_NODE, STATEMENT_LIST_NODE,
	BLOCK_NODE, PARAMETER_SECTION_NODE, PARAMETERS_NODE, IMPL_NODE, ENUM_DECL_NODE,
	FUNCTION_SIGNATURE_NODE, FUNCTION_DECL_NODE, VARIABLE_DECL_NODE, FUNCTION_CALL_NODE,
	DECLARATION_NODE, INC_DEC_STAT_NODE, RETURN_STAT_NODE, BREAK_STAT_NODE,
	CONTINUE_STAT_NODE, LEAVE_STAT_NODE, ASSIGNMENT_NODE, UNSTRUCTURED_STATEMENT_NODE,
	ELSE_STAT_NODE, IF_STAT_NODE, MATCH_CLAUSE_STAT, MATCH_STAT_NODE, FOR_STAT_NODE,
	STRUCTURED_STATEMENT_NODE, STATEMENT_NODE, TYPE_NODE, POINTER_FREE_NODE, TUPLE_TYPE_NODE,
	TUPLE_EXPR_NODE, OPTION_TYPE_NODE, 
	MACRO_NODE, USE_MACRO_NODE, LINKER_FLAG_MACRO_NODE, EXPR_STAT_NODE, ARRAY_INITIALIZER_NODE
} NodeType;

typedef enum {
	INFINITE_FOR_LOOP, WHILE_FOR_LOOP, INDEX_FOR_LOOP
} ForLoopType;

/**
 * Wrapper for a node
 */
typedef struct {
	void *data;
	NodeType type;
} Node;

/**
 * A list of identifiers, a common
 * production.
 */
typedef struct {
	Vector *values;
} IdentifierList;

/**
 * A node representing a literal.
 */
typedef struct {
	char *value;
	int type;
} Literal;

/**
 * A node representing an array
 * initializer, e.g: [ 1, 1, 1, 1 ]
 */
typedef struct {
	Vector *values;
} ArrayInitializer;

/**
 * A name of a type.
 */
typedef struct {
	char *name;
	int type;
} TypeName;

/**
 * A pointer type, i.e `int ^`
 */
typedef struct {
	Type *type;
} PointerType;

/**
 * Use statement node
 */
typedef struct {
	Vector *files;
} UseMacro;

typedef struct {
	char *flag;
} LinkerFlagMacro;

/**
 * Implementation node
 */
typedef struct {
	// what we're implementing
	char *name;	

	// what we're aliasing it as
	char *as;

	// functions
	Vector *funcs;
} Impl;

typedef struct {
	char *name;
	Expression *val;
} EnumItem;

typedef struct {
	char *name;
	Vector *items;
} EnumDecl;

typedef struct {
	Vector *types;
} TupleType;

typedef struct {
	Type *type;
} OptionType;

typedef struct {
	Vector *values;
} TupleExpr;

/**
 * An array type
 * i.e: []int;
 */
typedef struct {
	Type *type;
} ArrayType;

/**
 * A type literal, i.e Array or Pointer. These
 * are not what you think they are and need
 * fixing. I don't even  remember myself, well
 * I do I just can't be bothered to write it. Yet
 * I've written all this. Wow.
 */
typedef struct {
	ArrayType *arrayType;
	PointerType *pointerType;
	TupleType *tupleType;
	OptionType *optionType; 
	int type;
} TypeLit;

/**
 * A type, which can be a type name,
 * array type, pointer type, or a structure
 * declaration.
 */
struct s_Type {
	TypeName *typeName;
	TypeLit *typeLit;
	int type;
};

/**
 * A node representing a function call
 */
typedef struct {
	Vector *callee;
	Vector *arguments;
} Call;

/**
 * A node representing a unary expression, for instance
 * +5, or -5.
 */
typedef struct s_UnaryExpr {
	Expression *lhand;
	char *unaryOp;
} UnaryExpr;

typedef struct {
	Expression *lhand;
	char *binaryOp;
	Expression *rhand;
} BinaryExpr;

/**
 * Inheritance "emulation", an Expression Node is the parent
 * of all expressions.
 */
struct s_Expression {
	Call *call;
	Type *type;
	Literal *lit;
	BinaryExpr *binary;
	UnaryExpr *unary;
	ArrayInitializer *arrayInitializer;
	TupleExpr *tupleExpr;
	int exprType;
};


/**
 * Field declaration for a struct
 */
typedef struct {
	Type *type;
	char *name;
	bool mutable;
} FieldDecl;

/**
 * List of field declarations, this could probably
 * just be in the Struct Decl as a list.
 */
typedef struct {
	Vector *members;
} FieldDeclList;

/**
 * A node representing a user-defined data structure.
 */
typedef struct {
	FieldDeclList *fields;
	char *name;
} StructDecl;

/**
 * A list of statements
 */
typedef struct {
	Vector *stmts;
} StatementList;

/**
 * A node representing a block. The grammar
 * suggests that a single statement block, denoted with
 * an arrow, only holds one statement, but we can just use
 * the same list.
 */
typedef struct {
	StatementList *stmtList;
	bool singleStatementBlock;
} Block;

/**
 * This node represents a parameter in
 * a function signature
 */
typedef struct {
	Type *type;
	char *name;
	bool mutable;
	Expression *optionalExpr;
} ParameterSection;

/**
 * Parameters for a function, again potentially
 * redundant, just a wrapper for a vector...
 */
typedef struct {
	Vector *paramList;
	bool variadic;
} Parameters;

/**
 * A function signature or prototype.
 */
typedef struct {
	char *name;
	char *owner;
	char *ownerArg;
	Parameters *parameters;
	bool mutable;
	Type *type;
} FunctionSignature;

/**
 * A function, which consists
 * of a function prototype and an optional block.
 */
typedef struct {
	// the function signature of the func
	FunctionSignature *signature;
	
	// body for func
	Block *body;

	// how many required args for func
	int numOfRequiredArgs;

	// if the function decl is a prototype
	bool prototype;
} FunctionDecl;

/**
 * A node representing a variable declaration
 */
typedef struct {
	// if the variable declaration is mutable
	bool mutable;
	
	// if it's a pointer
	bool pointer;

	// is it being assigned a value
	bool assigned;

	// if the type is infered
	bool inferred;
	
	// the decl type
	Type *type;
	
	// the decl name
	char *name;
	
	// the decl expr rhand value
	Expression *expr;
} VariableDecl;

/**
 * A node representing all declarations,
 * function/struct/variable...
 */
typedef struct {
	FunctionDecl *funcDecl;
	StructDecl *structDecl;
	VariableDecl *varDecl;
	EnumDecl *enumDecl;
	int type;
} Declaration;

/**
 * A statement representing an increment
 * or decrement.
 */
typedef struct {
	Expression *expr;
	int amount;
} IncDecStat;

/**
 * A return statement
 */
typedef struct {
	Expression *expr;
} ReturnStat;

/**
 * Node for an assignment
 */
typedef struct {
	char *iden;
	Expression *expr;
} Assignment;

/**
 * Node for all the "leave" statements,
 * e.g continue, return, break
 */
typedef struct {
	ReturnStat *retStmt;
	BreakStat *breakStmt;
	ContinueStat *conStmt;
	int type;
} LeaveStat;

typedef struct {
	char *name;
} PointerFree;

typedef struct {
	UseMacro *use;
	LinkerFlagMacro *linker;
	int type;
} Macro;

/**
 * Unstructured statements, i.e things that don't have
 * a body, i.e declaration, assignment...
 */
typedef struct {
	Declaration *decl;
	LeaveStat *leave;
	IncDecStat *incDec;
	Assignment *assignment;
	Call *call;
	Impl *impl;
	PointerFree *pointerFree;
	Expression *expr;
	int type;
} UnstructuredStatement;

/**
 * Else statement block
 */
typedef struct {
	Block *body;
} ElseStat;

/**
 * Else If Block
 */
typedef struct {
	Expression *condition;
	Block *body;
} ElseIfStat;

/**
 * If statement node, expr is
 * for a condition
 */
typedef struct {
	Expression *expr;
	Block *body;
	Vector *elseIfStmts;
	ElseStat *elseStmt;
} IfStat;

/**
 * A match clause for a match statement
 */
typedef struct {
	Expression *expr;
	Block *body;
} MatchClause;

/**
 * match statement
 */
typedef struct {
	Expression *expr;
	Vector *clauses;
} MatchStat;

/**
 * node representing a for loop
 */
typedef struct {
	Type *type;
	Expression *index;
	Expression *step;
	Block *body;
	int forType;
} ForStat;

/**
 * Structured statements, i.e for loops,
 * matches, etc..
 */
typedef struct {
	Block *block;
	IfStat *ifStmt;
	MatchStat *matchStmt;
	ForStat *forStmt;
	int type;
} StructuredStatement;

/**
 * Statements, structured and unstructured
 */
typedef struct {
	StructuredStatement *structured;
	UnstructuredStatement *unstructured;
	Macro *macro;
	int type;
} Statement;

Node *createNode(void *data, NodeType type);

Impl *createImpl(char *name, char *as);

UseMacro *createUseMacro();

ArrayInitializer *createArrayInitializer();

LinkerFlagMacro *createLinkerFlagMacro(char *flag);

IdentifierList *createIdentifierList();

Literal *createLiteral(char *value, int type);

TypeLit *createTypeLit();

UnaryExpr *createUnaryExpr();

OptionType *createOptionType(Type *type);

TupleType *createTupleType();

TupleExpr *createTupleExpr();

EnumItem *createEnumItem(char *name);

EnumDecl *createEnumDecl(char *name);

Call *createCall(Vector *callee);

Expression *createExpression();

BinaryExpr *createBinaryExpr();

TypeName *createTypeName(char *name);

ArrayType *createArrayType(Type *type);

PointerType *createPointerType(Type *type);

FieldDecl *createFieldDecl(Type *type, bool mutable);

FieldDeclList *createFieldDeclList();

StructDecl *createStructDecl(char *name);

StatementList *createStatementList();

Block *createBlock();

ParameterSection *createParameterSection(Type *type, bool mutable);

Parameters *createParameters();

FunctionSignature *createFunctionSignature(char *name, Parameters *params, bool mutable, Type *type);

FunctionDecl *createFunctionDecl();

VariableDecl *createVariableDecl(Type *type, char *name, bool mutable, Expression *rhand);

Declaration *createDeclaration();

IncDecStat *createIncDecStat(Expression *rhand, int inc);

ReturnStat *createReturnStat(Expression *rhand);

BreakStat *createBreakStat();

ContinueStat *createContinueStat();

LeaveStat *createLeaveStat();

Assignment *createAssignment(char *iden, Expression *rhand);

UnstructuredStatement *createUnstructuredStatement();

Macro *createMacro();

PointerFree *createPointerFree(char *name);

ElseStat *createElseStat();

ElseIfStat *createElseIfStat();

IfStat *createIfStat();

MatchClause *createMatchClause();

MatchStat *createMatchStat(Expression *rhand);

ForStat *createForStat();

StructuredStatement *createStructuredStatement();

Statement *createStatement();

Type *createType();

void cleanupAST(Vector *nodes);

void destroyNode(Node *node);

void destroyImpl(Impl *impl);

void destroyUseMacro(UseMacro *use);

void destroyArrayInitializer(ArrayInitializer *array);

void destroyLinkerFlagMacro(LinkerFlagMacro *linker);

void destroyIdentifierList(IdentifierList *list);

void destroyLiteral(Literal *lit);

void destroyUnaryExpr(UnaryExpr *rhand);

void destroyTupleType(TupleType *tuple);

void destroyOptionType(OptionType *type);

void destroyTupleExpr(TupleExpr *tuple);

void destroyEnumItem(EnumItem *item);

void destroyEnumDecl(EnumDecl *decl);

void destroyCall(Call *call);

void destroyExpression(Expression *rhand);

void destroyBinaryExpression(BinaryExpr *binary);

void destroyTypeName(TypeName *typeName);

void destroyArrayType(ArrayType *arrayType);

void destroyPointerType(PointerType *pointerType);

void destroyFieldDecl(FieldDecl *decl);

void destroyFieldDeclList(FieldDeclList *list);

void destroyStructDecl(StructDecl *decl);

void destroyStatementList(StatementList *list);

void destroyBlock(Block *block);

void destroyParameterSection(ParameterSection *param);

void destroyParameters(Parameters *params);

void destroyFunctionSignature(FunctionSignature *func);

void destroyFunctionDecl(FunctionDecl *decl);

void destroyVariableDecl(VariableDecl *decl);

void destroyDeclaration(Declaration *decl);

void destroyIncDecStat(IncDecStat *stmt);

void destroyReturnStat(ReturnStat *stmt);

void destroyBreakStat(BreakStat *stmt);

void destroyContinueStat(ContinueStat *stmt);

void destroyLeaveStat(LeaveStat *stmt);

void destroyAssignment(Assignment *assign);

void destroyUnstructuredStatement(UnstructuredStatement *stmt);

void destroyMacro(Macro *macro);

void destroyPointerFree(PointerFree *pntr);

void destroyElseStat(ElseStat *stmt);

void destroyElseIfStat(ElseIfStat *stmt);

void destroyIfStat(IfStat *stmt);

void destroyMatchClause(MatchClause *mclause);

void destroyMatchStat(MatchStat *match);

void destroyForStat(ForStat *stmt);

void destroyStructuredStatement(StructuredStatement *stmt);

void destroyStatement(Statement *stmt);

void destroyTypeLiteral(TypeLit *type);

void destroyType(Type *type);

#endif // __AST_H
