#include "Wallet.h"
#include <iostream>
#include "MerkelMain.h"
#include "UserManager.h"
#include "User.h"

int main()
{   
    UserManager userManager;

    // Show login/register menu first for Task 2
    userManager.showAuthMenu();

    // Once logged in, launch trading system
    User* loggedInUser = userManager.getCurrentUser();

    // Passing in the logged in user object to MerkelMain for Task 2
    MerkelMain app(*loggedInUser);
    app.init();

    return 0;

    
}
