#include "Wallet.h"
#include <iostream>
#include "MerkelMain.h"
#include "UserManager.h"
#include "User.h"

int main()
{   
    UserManager userManager;

    // Show login/register menu first
    userManager.showAuthMenu();

    // Once logged in, launch trading system
    User* loggedInUser = userManager.getCurrentUser();
    MerkelMain app(*loggedInUser);
    app.init();

    return 0;

    
}
