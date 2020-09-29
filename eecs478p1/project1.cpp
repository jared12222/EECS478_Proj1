/*
 * Contains the the apply function, the cofactors and quantification functions.
 *
 * For Project 1, implement
 * (1) apply, also handles probabilities
 * (2) negative_cofactor
 * (3) positive_cofactor, 
 * (4) boolean_difference
 * (5) sort by influence
 * (6) check probability equivalence
 */

#include "project1.h"

using namespace std;

// wrapper function to allow calling with the operation, i.e apply(bdd1, bdd2, "or")
bdd_ptr apply(bdd_ptr bdd1, bdd_ptr bdd2, string o)
{
  operation dop;
  if (!dop.set_operation(o))
  {
    return 0;
  }
  
  return apply(bdd1, bdd2, dop);
}

// apply implements an arbitrary operation (specified in op) on two BDDs
// bdd_tables is used to handle the book keeping (see bdd_tables.h).
//
// apply works recursively one the idea that given an arbitrary operation $, 
// and functions f and g: f $ g = a'(fa' $ ga') + a(fa $ ga), 
// where a is a variable, fa' is the negative cofactor etc.
bdd_ptr apply(bdd_ptr bdd1, bdd_ptr bdd2, operation &op)
{
  // get reference to tables
  bdd_tables& tables = bdd_tables::getInstance();
  
  // ... your code goes here
  // change the return value when you're finished
  bdd_ptr node,neg_n,pos_n;
  string str_op = bdd1->var + op.get_operation() + bdd2->var;
  // cout << "computed node " << str_op << endl;
  if( tables.find_in_computed_table(str_op,bdd1,bdd2)!= 0 )
    return tables.find_in_computed_table(str_op,bdd1,bdd2);
  node = op(bdd1,bdd2);
  if( node != 0 )
  {
    tables.insert_computed_table(str_op,bdd1,bdd2,node);
    return node;
  }
  else if ( bdd1->is_terminal() )
  {
    neg_n = apply(bdd1,bdd2->neg_cf,op.get_operation());
    pos_n = apply(bdd1,bdd2->pos_cf,op.get_operation());
  }
  else if ( bdd2->is_terminal() )
  {
    neg_n = apply(bdd1->neg_cf,bdd2,op.get_operation());
    pos_n = apply(bdd1->pos_cf,bdd2,op.get_operation());
  }
  else
  {
    neg_n = apply(bdd1->neg_cf,bdd2->neg_cf,op.get_operation());
    pos_n = apply(bdd1->pos_cf,bdd2->pos_cf,op.get_operation());
  }

  // fill in childs into node
  node->neg_cf = neg_n;
  node->pos_cf = pos_n;
  
  return node;
}

// negative_cofactor takes the BDD pointed to by np, 
// and returns the negative cofactor with respect to var.
bdd_ptr negative_cofactor(bdd_ptr np, char var)
{
  // get reference to tables
  bdd_tables& tables = bdd_tables::getInstance();
  
  //... your code goes here
  
  // once you add your code, remove this and return your own result
  return np; 
}

// posative_cofactor takes the BDD pointed to by np, 
// and returns the posative cofactor with respect to var.
bdd_ptr positive_cofactor(bdd_ptr np, char var)
{
  // get reference to tables
  bdd_tables& tables = bdd_tables::getInstance();
  
  //... your code goes here
  
  // once you add your code, remove this and return your own result
  return np; 
}

// boolean_difference takes the BDD pointed to by np, 
// and returns the boolean difference with respect to var.
bdd_ptr boolean_difference(bdd_ptr np, char var)
{
  // get reference to tables
  bdd_tables& tables = bdd_tables::getInstance();
  
  //... your code goes here
  
  // once you add your code, remove this and return your own result
  return np; 
}


// sort_by_influence calculates the influence of all the variables in np
// and displays them in descending order (most influent variable is
// shown first). For this task you can assume the number of variable
// is no greater than 20.
bdd_ptr sort_by_influence(bdd_ptr np)
{
  //... your code goes here
  
  // this function does not alter the current node, so np must be
  // returned at the end
  return np; 
}

//check if two bdd are p equivalence, return true if they are p equivalence
//otherwise return false.
bool check_Probability_equivalence(bdd_ptr bdd1, bdd_ptr bdd2)
{
  bool is_P_equivalence = false;
  //... your code goes here
  //
  // this function does not alter the current node, so np must be
  // returned at the end
  return is_P_equivalence;
}

