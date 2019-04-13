/*
    Copyright 2018 Joel Svensson	svenssonjoel@yahoo.se

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "symrepr.h"
#include "heap.h"
#include "builtin.h"
#include "print.h"

typedef struct s_builtin_function{
  VALUE sym;
  bi_fptr fun_ptr;
  struct s_builtin_function* next;
} builtin_function_t;

builtin_function_t* function_list = NULL;

////////////////////////////////////////////////////////////
// Built in predicates (numberp, symbolp and such)
bool is_number(VALUE arg) {
  TYPE t = type_of(arg);
  return (t == VAL_TYPE_I28 ||
	  t == VAL_TYPE_U28 ||
	  t == PTR_TYPE_I32 ||
	  t == PTR_TYPE_U32 ||
	  t == PTR_TYPE_F32);
}

VALUE bi_fun_numberp(VALUE args) {
  if (is_number(car(args)) && length(args) == 1) {
    return enc_sym(symrepr_true());
  }
  return enc_sym(symrepr_nil());
}


////////////////////////////////////////////////////////////
// Built in functions
VALUE bi_fun_car(VALUE args) {
  return car(car(args));
}

VALUE bi_fun_cdr(VALUE args) {
  return cdr(car(args));
}

VALUE bi_fun_cons(VALUE args) {
  VALUE a = car(args);
  VALUE b = car(cdr(args));
  return cons(a,b);
}

VALUE bi_fun_reverse(VALUE args) {
  VALUE xs = car(args);
  return reverse(xs);
}

/*
 * get_max_num_type
 * Returns: success or failure (1 or 0).
 * Result is stored in argument 2.
 */
int get_max_num_type(VALUE args, TYPE *type) {

  TYPE max_type = 0;
  VALUE curr = args;

  while ( type_of(curr) == PTR_TYPE_CONS ) {
    if (!is_number(car(curr)))
      return 0;

    if ( type_of(car(curr)) > max_type) {
      max_type = type_of(car(curr));
    }
    curr = cdr(curr);
  }
  *type = max_type;
  return 1;
}

int get_as_int(VALUE v, INT *r) {
  VALUE tmp;
  FLOAT f_tmp;
  switch (type_of(v)) {
  case VAL_TYPE_I28:
    *r =  dec_i(v);
    break;
  case VAL_TYPE_U28:
    *r = (INT)dec_u(v);
    break;
  case PTR_TYPE_I32:
    *r = (INT)car(v);
    break;
  case PTR_TYPE_U32:
    *r = (INT)car(v);
    break;
  case PTR_TYPE_F32:
    tmp = car(v);
    // f_tmp = *(FLOAT*)&tmp;
    memcpy(&f_tmp, &tmp, sizeof(FLOAT));
    *r = (INT)f_tmp;
    break;
  default:
    return 0;
  }
  return 1;
}

int get_as_uint(VALUE v, UINT *r) {
  VALUE tmp;
  FLOAT    f_tmp;
  switch (type_of(v)) {
  case VAL_TYPE_I28:
    *r =  (UINT)dec_i(v);
    break;
  case VAL_TYPE_U28:
    *r = dec_u(v);
    break;
  case PTR_TYPE_I32:
    *r = (UINT)car(v);
    break;
  case PTR_TYPE_U32:
    *r = car(v);
    break;
  case PTR_TYPE_F32:
    tmp = car(v);
    // f_tmp = *(FLOAT*)&tmp;
    memcpy(&f_tmp, &tmp, sizeof(FLOAT));
    *r = (UINT)f_tmp;
    break;
  default:
    return 0;
  }
  return 1;
}

int get_as_float(VALUE v, FLOAT *r) {
  VALUE tmp;
  switch (type_of(v)) {
  case VAL_TYPE_I28:
    *r =  (FLOAT)dec_i(v);
    break;
  case VAL_TYPE_U28:
    *r = (FLOAT)dec_u(v);
    break;
  case PTR_TYPE_I32:
    *r = (FLOAT)((INT)car(v));
    break;
  case PTR_TYPE_U32:
    *r = (FLOAT)car(v);
    break;
  case PTR_TYPE_F32:
    tmp = car(v);
    //*r = *(FLOAT*)&tmp;
    memcpy(r, &tmp, sizeof(FLOAT));
    break;
  default:
    return 0;
  }
  return 1;
}

VALUE bi_fun_sum(VALUE args) {
  VALUE   curr = args;
  INT     i_sum = 0;
  UINT    u_sum = 0;
  FLOAT   f_sum = 0.0;

  UINT tmp;
  UINT float_enc;
  TYPE max_type;

  if (! (get_max_num_type(args, &max_type)))
    return enc_sym(symrepr_terror());

  switch (max_type) {

  case VAL_TYPE_I28:
    while(type_of(curr) == PTR_TYPE_CONS) {
      VALUE v = car(curr);
      INT   r;
      get_as_int(v,&r);
      i_sum += r;
      curr = cdr(curr);
    }
    return enc_i(i_sum);

  case VAL_TYPE_U28:
    while(type_of(curr) == PTR_TYPE_CONS) {
      VALUE v = car(curr);
      UINT r;
      get_as_uint(v,&r);
      u_sum += r;
      curr = cdr(curr);
    }
    return enc_u(u_sum);

  case PTR_TYPE_I32:
    while(type_of(curr) == PTR_TYPE_CONS) {
      VALUE v = car(curr);
      INT r;
      get_as_int(v,&r);
      i_sum += r;
      curr= cdr(curr);
    }
    tmp = cons(i_sum,enc_sym(SPECIAL_SYM_I32));
    return set_ptr_type(tmp, PTR_TYPE_I32);

  case PTR_TYPE_U32:
    while(type_of(curr) == PTR_TYPE_CONS) {
      VALUE v = car(curr);
      UINT  r;
      get_as_uint(v,&r);
      u_sum += r;
      curr= cdr(curr);
    }
    tmp = cons(u_sum,enc_sym(SPECIAL_SYM_U32));
    if (type_of(tmp) == VAL_TYPE_SYMBOL) // an error
      return tmp;
    return set_ptr_type(tmp, PTR_TYPE_U32);

  case PTR_TYPE_F32:
    while(type_of(curr) == PTR_TYPE_CONS) {
      VALUE v = car(curr);
      FLOAT r;
      get_as_float(v,&r);
      f_sum += r;
      curr = cdr(curr);
    }
    memcpy(&tmp, &f_sum, sizeof(UINT));
    float_enc = cons(tmp,enc_sym(SPECIAL_SYM_F));
    if (type_of(float_enc) == VAL_TYPE_SYMBOL) // an error
      return float_enc;
    float_enc = set_ptr_type(float_enc, PTR_TYPE_F32);
    return float_enc;
  }

  return enc_sym(symrepr_eerror());
}

VALUE bi_fun_sub(VALUE args) {

  UINT  tmp;
  UINT  enc;
  FLOAT f_res;
  UINT  u_res;
  INT   i_res;
  TYPE  max_type;

  if (!(get_max_num_type(args, &max_type)))
    return enc_sym(symrepr_terror());

  unsigned int n = length(args);

  if (n < 1) return enc_sym(symrepr_eerror());

  // A negation
  if (n == 1) {
    switch (max_type) {

    case VAL_TYPE_I28:
      i_res = -dec_i(car(args));
      return enc_i(i_res);

    case VAL_TYPE_U28:
      u_res = -dec_u(car(args));
      return enc_u(u_res);

    case PTR_TYPE_I32:
      i_res = -(INT)car(car(args));
      enc = cons(i_res, enc_sym(SPECIAL_SYM_I32));
      if (type_of(enc) == VAL_TYPE_SYMBOL) // an error
	return enc;
      enc = set_ptr_type(enc,PTR_TYPE_I32);
      return enc;

    case PTR_TYPE_U32:
      u_res = -car(car(args));
      enc = cons(u_res, enc_sym(SPECIAL_SYM_U32));
      if (type_of(enc) == VAL_TYPE_SYMBOL) // an error
	return enc;
      enc = set_ptr_type(enc,PTR_TYPE_U32);
      return enc;

    case PTR_TYPE_F32:
      enc = car(car(args));
      memcpy(&f_res, &enc, sizeof(FLOAT));
      f_res = - f_res;
      memcpy(&tmp, &f_res, sizeof(UINT));
      enc = cons(tmp,enc_sym(SPECIAL_SYM_F));
      if (type_of(enc) == VAL_TYPE_SYMBOL) // an error
	return enc;
      enc = set_ptr_type(enc, PTR_TYPE_F32);
    return enc;
      break;
    }
  }

  VALUE v = car(args);
  VALUE curr = cdr(args);

  switch (max_type) {
  case VAL_TYPE_I28:
    get_as_int(v, &i_res);

    while(type_of(curr) == PTR_TYPE_CONS) {
      UINT v = car(curr);
      INT r;
      get_as_int(v,&r);
      i_res -= r;
      curr = cdr(curr);
    }
    return enc_i(i_res);

  case VAL_TYPE_U28:
    get_as_uint(v, &u_res);

    while(type_of(curr) == PTR_TYPE_CONS) {
      UINT v = car(curr);
      UINT r;
      get_as_uint(v,&r);
      u_res -= r;
      curr = cdr(curr);
    }
    return enc_u(u_res);

  case PTR_TYPE_I32:
    get_as_int(v, &i_res);

    while(type_of(curr) == PTR_TYPE_CONS) {
      UINT v = car(curr);
      INT r;
      get_as_int(v, &r);
      i_res -= r;
      curr = cdr(curr);
    }
    enc = cons(i_res, enc_sym(SPECIAL_SYM_I32));
    if (type_of(enc) == VAL_TYPE_SYMBOL) // an error
      return enc;
    enc = set_ptr_type(enc,PTR_TYPE_I32);
    return enc;

  case PTR_TYPE_U32:
    get_as_uint(v, &u_res);

    while(type_of(curr) == PTR_TYPE_CONS) {
      UINT v = car(curr);
      UINT r;
      get_as_uint(v, &r);
      u_res -= r;
      curr = cdr(curr);
    }
    enc = cons(u_res, enc_sym(SPECIAL_SYM_U32));
    if (type_of(enc) == VAL_TYPE_SYMBOL) // an error
      return enc;
    enc = set_ptr_type(enc,PTR_TYPE_U32);
    return enc;

  case PTR_TYPE_F32:
    get_as_float(v, &f_res);

    while(type_of(curr) == PTR_TYPE_CONS) {
      UINT v = car(curr);
      FLOAT r;
      get_as_float(v, &r);
      f_res -= r;
      curr = cdr(curr);
    }
    memcpy(&tmp, &f_res, sizeof(UINT));
    enc = cons(tmp,enc_sym(SPECIAL_SYM_F));
    if (type_of(enc) == VAL_TYPE_SYMBOL) // an error
      return enc;
    enc = set_ptr_type(enc, PTR_TYPE_F32);
    return enc;
  }

  return enc_sym(symrepr_eerror());
}


VALUE bi_fun_gt(VALUE args) {
  VALUE a1 = car(args);
  VALUE a2 = car(cdr(args));
  INT i1, i2;
  UINT u1, u2;
  FLOAT f1, f2;

  TYPE max_type = (type_of(a1) >= type_of(a2)) ? type_of(a1) : type_of(a2);

  switch (max_type) {

  case VAL_TYPE_I28:
    get_as_int(a1, &i1);
    get_as_int(a2, &i2);
    if (i1 > i2) return enc_sym(symrepr_true());
    else return enc_sym(symrepr_nil());
  case VAL_TYPE_U28:
    get_as_uint(a1, &u1);
    get_as_uint(a2, &u2);
    if (u1 > u2) return enc_sym(symrepr_true());
    else return enc_sym(symrepr_nil());
  case PTR_TYPE_I32:
    get_as_int(a1, &i1);
    get_as_int(a2, &i2);
    if (i1 > i2) return enc_sym(symrepr_true());
    else return enc_sym(symrepr_nil());
  case PTR_TYPE_U32:
    get_as_uint(a1, &u1);
    get_as_uint(a2, &u2);
    if (u1 > u2) return enc_sym(symrepr_true());
    else return enc_sym(symrepr_nil());
  case PTR_TYPE_F32:
    get_as_float(a1, &f1);
    get_as_float(a2, &f2);
    if (f1 > f2) return enc_sym(symrepr_true());
    else return enc_sym(symrepr_nil());
  }

  return enc_sym(symrepr_eerror());
}

VALUE bi_fun_lt(VALUE args) {
  VALUE a1 = car(args);
  VALUE a2 = car(cdr(args));
  INT i1, i2;
  UINT u1, u2;
  FLOAT f1, f2;

  TYPE max_type = (type_of(a1) >= type_of(a2)) ? type_of(a1) : type_of(a2);

  switch (max_type) {

  case VAL_TYPE_I28:
    get_as_int(a1, &i1);
    get_as_int(a2, &i2);
    if (i1 < i2) return enc_sym(symrepr_true());
    else return enc_sym(symrepr_nil());
  case VAL_TYPE_U28:
    get_as_uint(a1, &u1);
    get_as_uint(a2, &u2);
    if (u1 < u2) return enc_sym(symrepr_true());
    else return enc_sym(symrepr_nil());
  case PTR_TYPE_I32:
    get_as_int(a1, &i1);
    get_as_int(a2, &i2);
    if (i1 < i2) return enc_sym(symrepr_true());
    else return enc_sym(symrepr_nil());
  case PTR_TYPE_U32:
    get_as_uint(a1, &u1);
    get_as_uint(a2, &u2);
    if (u1 < u2) return enc_sym(symrepr_true());
    else return enc_sym(symrepr_nil());
  case PTR_TYPE_F32:
    get_as_float(a1, &f1);
    get_as_float(a2, &f2);
    if (f1 < f2) return enc_sym(symrepr_true());
    else return enc_sym(symrepr_nil());
  }

  return enc_sym(symrepr_eerror());
}

VALUE bi_fun_num_eq(VALUE args) {
  VALUE a1 = car(args);
  VALUE a2 = car(cdr(args));
  INT   i1, i2;
  UINT  u1, u2;
  FLOAT f1, f2;

  TYPE max_type = (type_of(a1) >= type_of(a2)) ? type_of(a1) : type_of(a2);

  switch (max_type) {

  case VAL_TYPE_I28:
    get_as_int(a1, &i1);
    get_as_int(a2, &i2);
    if (i1 == i2) return enc_sym(symrepr_true());
    else return enc_sym(symrepr_nil());
  case VAL_TYPE_U28:
    get_as_uint(a1, &u1);
    get_as_uint(a2, &u2);
    if (u1 == u2) return enc_sym(symrepr_true());
    else return enc_sym(symrepr_nil());
  case PTR_TYPE_I32:
    get_as_int(a1, &i1);
    get_as_int(a2, &i2);
    if (i1 == i2) return enc_sym(symrepr_true());
    else return enc_sym(symrepr_nil());
  case PTR_TYPE_U32:
    get_as_uint(a1, &u1);
    get_as_uint(a2, &u2);
    if (u1 == u2) return enc_sym(symrepr_true());
    else return enc_sym(symrepr_nil());
  case PTR_TYPE_F32:
    get_as_float(a1, &f1);
    get_as_float(a2, &f2);
    if (f1 == f2) return enc_sym(symrepr_true());
    else return enc_sym(symrepr_nil());
  }

  return enc_sym(symrepr_eerror());
}

int structural_equality(VALUE a, VALUE b) {

  if (!is_ptr(a) && !is_ptr(b)) {
    if (val_type(a) == val_type(b)){
      switch (val_type(a)) {
      case VAL_TYPE_SYMBOL:
	if (dec_sym(a) == dec_sym(b)) return 1;
        else return 0;
	break;
      case VAL_TYPE_I28:
	if (dec_i(a) == dec_i(b)) return 1;
	else return 0;
	break;
      case VAL_TYPE_U28:
	if (dec_u(a) == dec_u(b)) return 1;
	else return 0;
	break;
      case VAL_TYPE_CHAR:
	if (dec_char(a) == dec_char(b)) return 1;
	else return 0;
	break;
      default:
	return 0;
	break;
      }
    } else {
      return 0;
    }
  }

  if (is_ptr(a) && is_ptr(b)) {
    if (ptr_type(a) == ptr_type(b)) {
      if ( ptr_type(a) == PTR_TYPE_CONS ) {
	int car_eq = structural_equality(car(a),car(b));
	int cdr_eq = structural_equality(cdr(a),cdr(b));
	if ( car_eq && cdr_eq ) return 1;
	else return 0;
      }

      if (ptr_type(a) == PTR_TYPE_I32){
	INT ai = (INT)car(a);
	INT bi = (INT)car(b);
	  if (ai == bi) return 1;
	  else return 0;
      }

      if (ptr_type(a) == PTR_TYPE_U32){
	UINT au = car(a);
	UINT bu = car(b);
	  if (au == bu) return 1;
	  else return 0;
      }

      if (ptr_type(a) == PTR_TYPE_F32) {
	FLOAT af = (FLOAT)car(a);
	FLOAT bf = (FLOAT)car(b);
	  if (af == bf) return 1;
	  else return 0;
      }
      printf("TODO: Structural equality for this ptr type not implemented\n");
    } else {
      return 0;
    }
  }

  return 0;
}

VALUE bi_fun_eq(VALUE args) {
  VALUE a1 = car(args);
  VALUE a2 = car(cdr(args));

  return( structural_equality(a1, a2) ? enc_sym(symrepr_true()) : enc_sym(symrepr_nil()) );
}

VALUE bi_fun_gensym(VALUE args) {

  (void)args; // ignores any arguments
  
  // Takes no arguments!
  UINT gs;
  int res = gensym(&gs);

  if (res) return enc_sym(gs);
  return enc_sym(symrepr_eerror());
}

VALUE bi_fun_list(VALUE args) {
  VALUE t = enc_sym(symrepr_nil());
  VALUE list = enc_sym(symrepr_nil());
  VALUE curr = args;
  while (is_ptr(curr) && ptr_type(curr) == PTR_TYPE_CONS) {
    t = cons(car(curr),t);
    if (type_of(t) == VAL_TYPE_SYMBOL) // an error
      return t;

    curr = cdr(curr);
  }
  curr = t;
  while (is_ptr(curr) && ptr_type(curr) == PTR_TYPE_CONS) {
    list = cons(car(curr),list);
    if (type_of(list) == VAL_TYPE_SYMBOL) // an error
      return list; 

    curr = cdr(curr);
  }
  return list;
}

// Built in array functions

VALUE bi_fun_array_read(VALUE args) {
  // Args are: array, index
  VALUE arr = car(args);
  VALUE index = car(cdr(args));

  // Get array index
  UINT ix;
  INT  tmp;
  UINT res = enc_sym(symrepr_eerror());
  switch (type_of(index)) {
  case VAL_TYPE_U28:
    ix = dec_u(index);
    break;
  case VAL_TYPE_I28:
    tmp = (INT)dec_i(index);
    if (tmp < 0) return enc_sym(symrepr_eerror());
    ix = (UINT)tmp;
    break;
  case PTR_TYPE_U32:
    ix = car(index);
    break;
  case PTR_TYPE_I32:
    tmp = (INT)car(index);
    if (tmp < 0) return enc_sym(symrepr_eerror());
    ix = (UINT) tmp;
    break;
  default:
    return enc_sym(symrepr_nil());
  }

  if (type_of(arr) == PTR_TYPE_ARRAY) {
    array_t *array = (array_t*)car(arr);

    if (ix >= array->size) return enc_sym(symrepr_nil());

    switch(array->elt_type) {
    case VAL_TYPE_CHAR:
      res = enc_char((UINT) array->data.c[ix]);
      break;
    case VAL_TYPE_U28:
      res = enc_u(array->data.u32[ix]);
      break;
    case VAL_TYPE_I28:
      res = enc_i(array->data.i32[ix]);
      break;
    case PTR_TYPE_U32:
      res = cons(array->data.u32[ix], enc_sym(SPECIAL_SYM_U32));
      if (type_of(res) == VAL_TYPE_SYMBOL) // an error
	return res;
      res = set_ptr_type(res, PTR_TYPE_U32);
      break;
    case PTR_TYPE_I32:
      res = cons(array->data.i32[ix], enc_sym(SPECIAL_SYM_I32));
      if (type_of(res) == VAL_TYPE_SYMBOL) // an error
	return res;
      res = set_ptr_type(res, PTR_TYPE_I32);
      break;
    case PTR_TYPE_F32:
      res = cons(array->data.f[ix], enc_sym(SPECIAL_SYM_F));
      if (type_of(res) == VAL_TYPE_SYMBOL) // an error
	return res;
      res = set_ptr_type(res, PTR_TYPE_F32);
      break;
    default:
      printf("unknown type!\n");
      return enc_sym(symrepr_eerror());
    }
  }
  return res;
}

VALUE bi_fun_array_write(VALUE args) {
  // Args are: array, index, value
  VALUE arr = car(args);
  VALUE index = car(cdr(args));
  VALUE val = car(cdr(cdr(args)));
  UINT uv;
  FLOAT v;
  // Get array index
  UINT ix;
  INT tmp;
  switch (type_of(index)) {
  case VAL_TYPE_U28:
    ix = dec_u(index);
    break;
  case VAL_TYPE_I28:
    tmp = (INT)dec_i(index);
    if (tmp < 0) return enc_sym(symrepr_eerror());
    ix = (UINT) tmp;
    break;
  case PTR_TYPE_U32:
    ix = car(index);
    break;
  case PTR_TYPE_I32:
    tmp = (INT)car(index);
    if (tmp < 0) return enc_sym(symrepr_eerror());
    ix = (UINT) tmp;
    break;
  default:
    return enc_sym(symrepr_nil());
  }

  if (type_of(arr) == PTR_TYPE_ARRAY) {
    array_t *array = (array_t*)car(arr);
    if (type_of(val) != array->elt_type) return enc_sym(symrepr_nil());
    if (ix >= array->size) return enc_sym(symrepr_nil());

    switch(array->elt_type) {
    case VAL_TYPE_CHAR:
      array->data.c[ix] = dec_char(val);
      break;
    case VAL_TYPE_U28:
      array->data.u32[ix] = dec_u(val);
      break;
    case VAL_TYPE_I28:
      array->data.i32[ix] = dec_i(val);
      break;
    case PTR_TYPE_U32:
      array->data.u32[ix] = car(val);
      break;
    case PTR_TYPE_I32:
      array->data.i32[ix] = (INT)car(val);
      break;
    case PTR_TYPE_F32:
      uv = car(val);
      memcpy(&v, &uv, sizeof(FLOAT)); 
      array->data.f[ix] = v;
      break;
    default:
      printf("unknown type!\n");
      return enc_sym(symrepr_eerror());
    }
  }
  return enc_sym(symrepr_true());
}


// Interface functions

bi_fptr builtin_lookup_function(UINT sym){
  builtin_function_t *t = function_list;

  while (t != NULL) {
    if ( t->sym == sym ) {
      return t->fun_ptr;
    }
    t = t->next;
  }
  return NULL;
}

int builtin_add_function(char *sym_str, bi_fptr fun_ptr){

  VALUE symbol;
  int res = symrepr_addsym(sym_str, &symbol);

  if ( !res ) {
    return 0;
  }

  builtin_function_t *bi = malloc(sizeof(builtin_function_t));

  if ( !bi ) return 0;

  bi->sym = symbol;
  bi->fun_ptr = fun_ptr;
  bi->next = function_list;

  function_list = bi;
  return 1;
}

int builtin_init(void) {
  int res = 1;

  res &= builtin_add_function("+", bi_fun_sum);
  res &= builtin_add_function("-", bi_fun_sub);
  res &= builtin_add_function("car", bi_fun_car);
  res &= builtin_add_function("cdr", bi_fun_cdr);
  res &= builtin_add_function("cons", bi_fun_cons);
  res &= builtin_add_function(">", bi_fun_gt);
  res &= builtin_add_function("<", bi_fun_lt);
  res &= builtin_add_function("=", bi_fun_eq);
  res &= builtin_add_function("num-eq", bi_fun_num_eq);
  res &= builtin_add_function("gensym", bi_fun_gensym);
  res &= builtin_add_function("list", bi_fun_list);
  res &= builtin_add_function("reverse", bi_fun_reverse);
  res &= builtin_add_function("array-read", bi_fun_array_read);
  res &= builtin_add_function("array-write", bi_fun_array_write);
  res &= builtin_add_function("numberp", bi_fun_numberp);
  return res;
}

void builtin_del(void) {
  builtin_function_t *curr = function_list;
  builtin_function_t *t;
  while (curr) {
    t = curr;
    curr = curr->next;
    free(t);
  }
}

VALUE built_in_gen_env(void) {

  builtin_function_t* curr = function_list;

  VALUE env = enc_sym(symrepr_nil());

  while (curr) {
    VALUE sym = enc_sym(curr->sym);
    env = cons(cons(sym,sym),env);
    if (type_of(env) == VAL_TYPE_SYMBOL) // an error
      return env;
    curr = curr->next;
  }

  return env;
}
