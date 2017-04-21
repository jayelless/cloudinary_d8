<?php /**
 * @file
 * Contains \Drupal\cloudinary_sdk\EventSubscriber\InitSubscriber.
 */

namespace Drupal\cloudinary_sdk\EventSubscriber;

use Symfony\Component\HttpKernel\KernelEvents;
use Symfony\Component\EventDispatcher\EventSubscriberInterface;

class InitSubscriber implements EventSubscriberInterface {

  /**
   * {@inheritdoc}
   */
  public static function getSubscribedEvents() {
    return [KernelEvents::REQUEST => ['onEvent', 0]];
  }

  public function onEvent($cloudinaryConfig = []) {
    if (!is_array($cloudinaryConfig)) {
      $cloudinaryConfig = cloudinary_sdk_config_load();
    }

    if (!empty($cloudinaryConfig)) {
      \Cloudinary::config($cloudinaryConfig);
      return TRUE;
    }

    return FALSE;
  }

}
