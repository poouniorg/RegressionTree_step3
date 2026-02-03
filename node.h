// node.h
enum Feature {
    WEIGHT_KG = 0,
    AGE,
    HEIGHT_CM,
    WAITING_TIME,
    FEATURE_COUNT=4
};

enum Operator {
    OP_LE,  // <=
    OP_LT,  // <
    OP_EQ,  // =
    OP_GE,  // >=
    OP_GT   // >
};

class Node {
private:
    //member variables
    bool is_leaf;
    float value;
    int left_id;
    int right_id;

public:
    //setter and getters
    void mark_as_leaf() {is_leaf= true;}
    void set_value(double v) {value= v;}
    void set_children(int l, int r) {left_id= l; right_id= r;}
    bool test_leaf() {return is_leaf;}
    float get_value() {return value;}
    int get_left() {return left_id;}
    int get_right() {return right_id;}

    // Parsed condition (for internal nodes)
    Feature feature;
    Operator op;
    float threshold;
    // constructor
    Node();
    // member functions
    bool parse_condition(char* cond);
    bool eval_condition(float features[FEATURE_COUNT]);
};