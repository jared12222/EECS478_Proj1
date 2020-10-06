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
  // cout << "var " << bdd1->var << bdd2->var << endl;
  if( bdd1->var >  bdd2->var )
  {
    // Reorganized variables order
    bdd_ptr buff = bdd1;
    bdd1 = bdd2;
    bdd2 = buff;
  }

  bdd_ptr node,neg_n,pos_n;
  string str_op = bdd1->var + op.get_operation() + bdd2->var;
  char var;

  if( tables.find_in_computed_table(op.get_operation(),bdd1,bdd2)!= 0 )
    return tables.find_in_computed_table(str_op,bdd1,bdd2);
  
  if( bdd1->is_terminal() && bdd2->is_terminal() ) // Could be computed
  {
    // cout << "Node could be computed!" << endl;
    node = op(bdd1,bdd2);
    tables.insert_computed_table(str_op,bdd1,bdd2,node);
    // cout << "Result: " << node << endl;
    return node;
  }
  else if( bdd1->is_terminal() == 1 )
  {
    // cout << "bdd1 is terminal" << endl;
    neg_n = apply(bdd1,bdd2->neg_cf,op.get_operation());
    pos_n = apply(bdd1,bdd2->pos_cf,op.get_operation());
    var = bdd2->var;
  }
  else if ( bdd2->is_terminal() == 1 )
  {
    // cout << "bdd2 is terminal" << endl;
    neg_n = apply(bdd1->neg_cf,bdd2,op.get_operation());
    pos_n = apply(bdd1->pos_cf,bdd2,op.get_operation());
    var = bdd1->var;
  }
  else  // Both bdd aren't terminal
  {
    // cout << "No terminal" << endl;
    if( bdd1->var != bdd2->var )
    {
      neg_n = apply(bdd1->neg_cf,bdd2,op.get_operation());
      pos_n = apply(bdd1->pos_cf,bdd2,op.get_operation());
      var = bdd1->var;
    }
    else{
      neg_n = apply(bdd1->neg_cf,bdd2->neg_cf,op.get_operation());
      pos_n = apply(bdd1->pos_cf,bdd2->pos_cf,op.get_operation());
      var = bdd1->var;
    }
  }
  
  node = tables.find_in_unique_table(var,neg_n,pos_n);
  if( node == 0 )
    node = tables.create_and_add_to_unique_table(var,neg_n,pos_n);
  node->probability = (node->neg_cf->probability+node->pos_cf->probability)/2;
  return node;
}

// negative_cofactor takes the BDD pointed to by np, 
// and returns the negative cofactor with respect to var.
bdd_ptr negative_cofactor(bdd_ptr np, char var)
{
  // get reference to tables
  bdd_tables& tables = bdd_tables::getInstance();
  
  //... your code goes here
  bdd_ptr neg,pos;
  // if not terminal: do something
  if( np->is_terminal()== false )
  {
    // if var matched
    if( np->var == var ){
      // cout << "Found!" << endl << np->neg_cf << endl;
      np = np->neg_cf;
    }
    else // if var not matched
    {
      neg = negative_cofactor(np->neg_cf,var);
      pos = negative_cofactor(np->pos_cf,var);
      np = tables.create_and_add_to_unique_table(np->var,neg,pos);
    }
  }
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
  bdd_ptr neg,pos;
  // if not terminal: do something
  if( np->is_terminal()==false )
  {
    // if var matched
    if( np->var == var ){
      // cout << "Found!" << endl << np->pos_cf << endl;
      np = np->pos_cf;
    }
    else // if var not matched
    {
      neg = positive_cofactor(np->neg_cf,var);
      pos = positive_cofactor(np->pos_cf,var);
      np = tables.create_and_add_to_unique_table(np->var,neg,pos);
    }
  }
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
  bdd_ptr neg,pos;
  operation op("xor");
  neg = negative_cofactor(np,var);
  pos = positive_cofactor(np,var);
  np = apply(neg,pos,op);
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
  bdd_ptr node;
  char  var_array[20],ch_buff;
  float prob_array[20],prob_buff;
  int in=0;

  // Finding variables and calculate corresponding influence
  for( char ch = 'A' ; ch <= 'Z' ; ch++ )
  {
    if(np->has_var(ch) == true)
    {
      node = boolean_difference(np,ch);
      var_array[in] = ch;
      prob_array[in] = node->probability;
      in++;
    }
  }
  for( char ch = 'a' ; ch <= 'z' ; ch++ )
  {
    if(np->has_var(ch) == true)
    {
      node = boolean_difference(np,ch);
      var_array[in] = ch;
      prob_array[in] = node->probability;
      in++;
    }
  }

  // Sorting
  for( int i = 0 ; i < in ; ++i )
  {
    for( int j = i ; j < in ; ++j )
    {
      if( prob_array[j] > prob_array[i] )
      {
        ch_buff = var_array[i];
        prob_buff = prob_array[i];
        var_array[i] = var_array[j];
        prob_array[i] = prob_array[j];
        var_array[j] = ch_buff;
        prob_array[j] = prob_buff;
      }
    }
  }
  for( int i = 0 ; i < in ; ++i )
  {
    cout <<var_array[i]<<','<<prob_array[i]<<endl;
  }

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
  if (bdd1->probability == bdd2->probability)
  {
    is_P_equivalence = true;
  }
  
  // this function does not alter the current node, so np must be
  // returned at the end
  return is_P_equivalence;
}

