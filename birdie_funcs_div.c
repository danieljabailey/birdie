#include "birdie_funcs_div.h"

#define INIT_ATOMIC_OP_FUNC(result)	\
	struct val_struct_t *out;	\
	char buf[20];	\
	int numWritten;	\
	unsigned totLen;	\
	out = createValStruct();	\
	out->valName = newString(result);	\
	out->valID = newString("Unidentified");	\

struct val_struct_t *intDivVal(int64_t a, struct val_struct_t *b){

	INIT_ATOMIC_OP_FUNC("Int Div Val")
	ITERLIST_PARSE_DEF(thisItem,outlist,thisOutItem)

	out->valueType = b->valueType;

	switch(b->valueType){
		case vtInt: out->valI = a / b->valI;
		break;
		case vtString:
			//TODO
			numWritten = snprintf(buf, 20, INT_SPEC, a);
			if (numWritten >= 0){
				totLen = numWritten + strlen(b->valS) + 1;
				out->valS = (char *)malloc(sizeof(char) * totLen);
				snprintf(out->valS, totLen, "%s%s", buf, b->valS);
			}
		break;
		case vtFloat: out->valF =  (double)a / b->valF;
		break;
		case vtList:
			freeVal(out);
			ITERLIST_PARSE_BEGIN(b->list,thisItem,outlist,thisOutItem)
				thisOutItem->item = intDivVal(a, thisItem->item);
			ITERLIST_PARSE_END(thisItem,thisOutItem)
			out = wrapList(outlist);
		break;
	}
	return out;
}

struct val_struct_t *floatDivVal(double a, struct val_struct_t *b){
	INIT_ATOMIC_OP_FUNC("Float Div Val")

	ITERLIST_PARSE_DEF(thisItem,outlist,thisOutItem)

	out->valueType = b->valueType;

	switch(b->valueType){
		case vtInt: out->valF = a / ((double)(b->valI));
		break;
		case vtString:
			//TODO
			numWritten = snprintf(buf, 20, FLOAT_SPEC, a);
			if (numWritten >= 0){
				totLen = numWritten + strlen(b->valS) + 1;
				out->valS = (char *)malloc(sizeof(char) * totLen);
				snprintf(out->valS, totLen, "%s%s", buf, b->valS);
			}
			out->valueType = vtString;
		break;
		case vtFloat: out->valF = a / b->valF;
		break;
		case vtList:
			freeVal(out);
			ITERLIST_PARSE_BEGIN(b->list,thisItem,outlist,thisOutItem)
				thisOutItem->item = floatDivVal(a, thisItem->item);
			ITERLIST_PARSE_END(thisItem,thisOutItem)
			out = wrapList(outlist);
		break;
	}
	return out;
}

struct val_struct_t *stringDivVal(char *a, struct val_struct_t *b){
	int num, len;
	char *strp;
	int count;
	INIT_ATOMIC_OP_FUNC("String Div Val")

	ITERLIST_PARSE_DEF(thisItem,outlist,thisOutItem)

	out->valueType = vtString;

	switch(b->valueType){
		case vtInt:
			//String divide integer
			num = b->valI;
			out->valS = (char *)malloc(sizeof(char) * (num+1));
			memcpy( out->valS, a, num );
			out->valS[num] = '\0';
		break;
		case vtString:
			//String divide string (count substrings in string)
			//Substring is b->valS
			//String is a
			len = strlen(a);
			strp=a;
			count = 0;
			while (strp<(a+len)){
				strp = strstr(strp, b->valS);
				if (strp!=NULL){
					count++;
					strp++;
				}
				else{
					break;
				}

			}
			out->valueType = vtInt;
			out->valI = count;
		break;
		case vtFloat:
			//String divide float
			num = floor(b->valF);
			out->valS = (char *)malloc(sizeof(char) * (num+1));
			memcpy( out->valS, a, num );
			out->valS[num] = '\0';
		break;
		case vtList:
			freeVal(out);
			ITERLIST_PARSE_BEGIN(b->list,thisItem,outlist,thisOutItem)
				thisOutItem->item = stringDivVal(a, thisItem->item);
			ITERLIST_PARSE_END(thisItem,thisOutItem)
			out = wrapList(outlist);
			out->valueType = vtList;
		break;
	}
	return out;
}

struct val_struct_t *listDivVal(struct val_struct_t *a, struct val_struct_t *b){

	ITERLIST_PARSE_DEF(thisItem,out,thisOutItem)

	ITERLIST_PARSE_BEGIN(a->list,thisItem,out,thisOutItem)
		thisOutItem->item = valDiv(thisItem->item, b);
	ITERLIST_PARSE_END(thisItem,thisOutItem)

	struct val_struct_t *output = wrapList(out);

	return output;
}

struct val_struct_t *valDiv(struct val_struct_t *a, struct val_struct_t *b){
	switch(a->valueType){
		case vtInt:
			return intDivVal(a->valI, b);
		case vtFloat:
			return floatDivVal(a->valF, b);
		case vtString:
			return stringDivVal(a->valS, b);
		case vtList:
			return listDivVal(a, b);
	}
	return NULL;
}
