//
// Created by Sam on 2018/2/13.
//

#include <parser/statement.h>
#include <parser/parser.h>
#include <utils/utils.h>
/**
 * 在现有实现基础上，实现update from子句
 *
 * 支持的update语法：
 *UPDATE Person SET Address = 'Zhongshan 23', City = 'Nanjing'
 *WHERE LastName = 'Wilson'
 * UPDATE <table_name> SET <field1> = <expr1>[, <field2 = <expr2>, ..]
 * WHERE <logical_expr>
 *
 * 解析获得 sql_stmt_update 结构
 */

/*TODO: parse_sql_stmt_update， update语句解析*/
sql_stmt_update *parse_sql_stmt_update(ParserT *parser){
    // fprintf(stderr, "TODO: update is not implemented yet. in parse_sql_stmt_update \n");
    char *tableName = NULL;
    arraylist *fields = arraylist_create();
    arraylist *fieldexpr = arraylist_create();


    //第一个单词"update"
    TokenT *token = parseNextToken(parser);
    if(!matchToken(parser, TOKEN_RESERVED_WORD, "update")){
        //strcpy(parser->parserMessage, "语法错误.");
        return NULL;
    }

    //表名
    token = parseNextToken(parser);
    if(token->type = TOKEN_WORD){
        tableName = new_id_name();
        strcpy(tableName,token->text);
        token = parseEatAndNextToken(parser);//
    }else {
        strcpy(parser->parserMessage,"invalid sql: missing table name.");
        return  NULL;
    }

    //别名为空
    TableReference_t *ref1 = TableReference_make(tableName,NULL);
    SRA_t *where = SRATable(ref1);

    //"set"
    token = parseNextToken(parser);
    if(!matchToken(parser, TOKEN_RESERVED_WORD, "set")){
        strcpy(parser->parserMessage, "语法错误.");
        return NULL;
    }

    //表达式
    token = parseNextToken(parser);
    if(token->type == TOKEN_WORD) {
        while(token->type == TOKEN_WORD) {
            //fields链表加入列名
            // char *fieldName = new_id_name();
            //strcpy(fieldName, token->text);
            //arraylist_add(fields, fieldName);
            arraylist_add(fields,token->text);
            //等号
            token = parseEatAndNextToken(parser);
            if (token->type == TOKEN_EQ) {
                token = parseEatAndNextToken(parser);
            } else {
                strcpy(parser->parserMessage,"invalid sql:missing =!");
                return NULL;
            }
            //fieldsexpr链表加入值
//            token = parseNextToken(parser);
//            if (token->type == TOKEN_STRING || token->type == TOKEN_DECIMAL) {
//                if (token->type == TOKEN_STRING) {
//                    /*去掉引号*/
//                    int len = strlen(token->text) - 1;
//                    char *v = token->text + 1;
//                    char *value = (char *) calloc(len, 1);
//                    strcpy(value, v);
//                    value[len - 1] = '\0';
//
//                    arraylist_add(fieldexpr, value);
//                } else {
//                    integer *i = (integer *) calloc(sizeof(integer *), 1);
//                    i->val = atoi(token->text);
//                    arraylist_add(fieldexpr, i);
//                }
//            } else {
//                strcpy(parser->parserMessage, "无效sql;缺失值");
//                return NULL;
//            }
            Expression *expr = parseExpressionRD(parser);
            arraylist_add(fieldexpr,expr);

            //逗号','
            token = parseNextToken(parser);
            if(token->type == TOKEN_COMMA){
                token = parseEatAndNextToken(parser);
            }else {
                break;
            }
        }
    }else {
        strcpy(parser->parserMessage,"无效sql;缺失列名");
        return NULL;
    }

    //"where"
    token = parseNextToken(parser);
    if(!matchToken(parser,TOKEN_RESERVED_WORD,"where")){
        strcpy(parser->parserMessage,"语法错误");
        return  NULL;
    }

    //where后表达式
    token = parseNextToken(parser);
    Expression *whereExpr = parseExpressionRD(parser);
    where = SRASelect(where,whereExpr);
    if (parser->parserStateType == PARSER_WRONG) {
        return NULL;
    }

    sql_stmt_update *sqlStmtUpdate = (sql_stmt_update *)calloc(sizeof(sql_stmt_update),1);
    sqlStmtUpdate->tableName = tableName;
    sqlStmtUpdate->fields = fields;
    sqlStmtUpdate->fieldsExpr = fieldexpr;
    SRA_print(where);
    sqlStmtUpdate->where = where;

    return  sqlStmtUpdate;

};
