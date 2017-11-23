//
//  PineStore.h
//  PineEntertainment
//
//  Created by Do Thanh Nghia on 10/15/15.
//
//

#ifndef __PineEntertainment__PineStore__
#define __PineEntertainment__PineStore__

#include <stdio.h>
#define IAP_PRODUCT_MAX (128)
class PineStore
{
    int _current_iap_num;
    char _product_id[IAP_PRODUCT_MAX][128];
    char _product_price[IAP_PRODUCT_MAX][128];
    bool _syn_appstore[IAP_PRODUCT_MAX];
public:
    PineStore();
    void AddNewProduct(char* proID);
    void SynPriceForProduct(char *proID, char* price);
    bool IsSynPriceForProduct(char *proID);
    char* GetPriceForProduct(char* proID);
    int GetNumIAP(){return _current_iap_num;};
    char* GetProductIdAt(int index){return _product_id[index];};
    
    
};
#endif /* defined(__PineEntertainment__PineStore__) */
