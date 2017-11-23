//
//  PineStore.cpp
//  PineEntertainment
//
//  Created by Do Thanh Nghia on 10/15/15.
//
//
#include "pch.h"
#include "PineStore.h"
#include <string.h>


PineStore::PineStore()
{
    _current_iap_num = 0;
    for(int i = 0; i < IAP_PRODUCT_MAX; i++)
    {
        sprintf(_product_id[i],"");
        sprintf(_product_price[i],"");
        _syn_appstore[i] = false;
    }
    
}
void PineStore::AddNewProduct(char* proID)
{
    //check exits
    if(_current_iap_num >= IAP_PRODUCT_MAX)
    {
        printf("\nOut of product max: %d",IAP_PRODUCT_MAX);
        return;
    }
    for(int i = 0; i < _current_iap_num; i++)
    {
        if(strcmp(_product_id[i],proID)==0)
        {
            printf("\nProduct: %s is available at index: %d",_product_id[i],i);
            return;
        }
    }
    sprintf((char*)_product_id[_current_iap_num],"%s", (const char*)proID);
     printf("\nAdd new product: %s at index: %d",_product_id[_current_iap_num],_current_iap_num);
    _current_iap_num++;
    
}
void PineStore::SynPriceForProduct(char *proID, char* price)
{
    for(int i = 0; i < _current_iap_num; i++)
    {
        if(strcmp(_product_id[i],proID)==0)
        {
            printf("Product: %s has been syned at index: %d",_product_id[i],i);
            sprintf(( char*)_product_price[i],"%s", (const char*)price);
            _syn_appstore[i] = true;
            return;
        }
    }
    
    printf("\nHave not Product to syn");

}
bool PineStore::IsSynPriceForProduct(char *proID)
{
    for(int i = 0; i < _current_iap_num; i++)
    {
        if(strcmp(_product_id[i],proID)==0 && _syn_appstore[i] == true)
        {
            return true;
        }
    }
    return false;
}
char* PineStore::GetPriceForProduct(char* proID)
{
    for(int i = 0; i < _current_iap_num; i++)
    {
        if(strcmp(_product_id[i],proID)==0 && _syn_appstore[i] == true)
        {
            return _product_price[i];
        }
    }
    return NULL;
}
