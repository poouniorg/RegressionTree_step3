// node.cpp
#include "node.h"
#include <sstream>
#include <string>
#include <iostream>
using namespace std;

Node::Node() 
    : is_leaf(false), value(0.0),
      feature(WEIGHT_KG), op(OP_LE), threshold(0.0),
      left_id(-1), right_id(-1)
{}

// parse_condition: parses string and stores structured data 
bool Node::parse_condition(char* cond) {
    istringstream iss(cond);
    string feat_str, op_str;
    float th;

    if (!(iss >> feat_str >> op_str >> th))
        return false;

    // Parse feature
    if (feat_str == "weight_kg")      feature = WEIGHT_KG;
    else if (feat_str == "age")       feature = AGE;
    else if (feat_str == "height_cm") feature = HEIGHT_CM;
    else if (feat_str == "waiting_time") feature = WAITING_TIME;
    else return false;

    // Parse operator
    if (op_str == "<=") op = OP_LE;
    else if (op_str == "<")  op = OP_LT;
    else if (op_str == "=")  op = OP_EQ;
    else if (op_str == ">=") op = OP_GE;
    else if (op_str == ">")  op = OP_GT;
    else return false;

    threshold = th;
    return true;
}

// eval_condition: uses pre-parsed members  
bool Node::eval_condition(float features[FEATURE_COUNT]) {
    float feat_val = features[feature];

    switch (op) {
        case OP_LE: return feat_val <= threshold;
        case OP_LT: return feat_val < threshold;
        case OP_EQ: return feat_val == threshold;
        case OP_GE: return feat_val >= threshold;
        case OP_GT: return feat_val > threshold;
        default:    return false;
    }
}