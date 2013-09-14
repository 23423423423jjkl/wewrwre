-- SUMMARY --

This module allow integrate Drupal Commerce store with Yandex.Market.
On cron run module generates XML-file with path yandex-market.xml.
There is no ability to change the path to the file for now.
For more information by Yandex.Market see
http://help.yandex.ru/partnermarket/yml/about-yml.xml

-- FEATURES --

Allow you integrate Drupal Commerce project with Yandex.Market.

-- REQUIREMENTS --

http://drupal.org/project/ctools
http://drupal.org/project/entity
http://drupal.org/project/commerce

-- INSTALLATION --

Install as usual, see http://drupal.org/node/70151 for further information.
For correctly working with images, apply the patch
http://drupal.org/node/1440928#comment-7239960

-- CONFIGURATION --

Go to admin/people/permissions#module-commerce_yandex_market to set
permission for module.
Go to admin/commerce/yandex-market/settings to configure the global settings.
Go to admin/commerce/yandex-market to add the display(s) which produce(s)
data for XML-file.
Run cron. Otherwise you can't see the file yandex-market.xml
Check for file on yandex-market.xml and enjoy!

-- CONTACT --

Current maintainer:
* Harbuzau Yauheni - http://drupal.org/user/2123020
