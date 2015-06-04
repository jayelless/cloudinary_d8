<?php

/**
 * @file
 * Admin page callback file for the cloudinary_sdk module.
 */

/**
 * Returns form items for the cloudinary account settings page.
 *
 * Menu callback for admin/config/media/cloudinary.
 *
 * @return array
 *   A form for the cloudinary account settings page.
 */
function cloudinary_sdk_settings($form, &$form_state) {
  // Make sure Cloudinary SDK installed.
  // If not, display messages and disable API settings.
  list($status, $version, $error_message) = cloudinary_sdk_check(TRUE);
  $disabled = ($status != CLOUDINARY_SDK_LOADED);

  if ($status == CLOUDINARY_SDK_NOT_LOADED) {
    drupal_set_message(t('Please make sure the Cloudinary SDK library is installed in the libraries directory.'), 'error');
    if ($error_message) {
      drupal_set_message($error_message, 'error');
    }
  }
  elseif ($status == CLOUDINARY_SDK_OLD_VERSION) {
    drupal_set_message(
      t(
        'Please make sure the Cloudinary SDK library installed is @version or greater. Current version is @current_version.',
        array(
          '@version' => CLOUDINARY_SDK_MINIMUM_VERSION,
          '@current_version' => $version,
        )
      ),
      'warning'
    );
  }

  // Build API settings form.
  $form = array();

  $form['settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('API Settings'),
    '#collapsible' => TRUE,
    '#collapsed' => $disabled,
    '#description' => t('In order to check the validity of the API, system will be auto ping your Cloudinary account after change API settings.'),
  );

  $form['settings']['cloudinary_sdk_cloud_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Cloud name'),
    '#required' => TRUE,
    '#default_value' => variable_get('cloudinary_sdk_cloud_name', ''),
    '#description' => t('Cloud name of Cloudinary.'),
    '#disabled' => $disabled,
  );

  $form['settings']['cloudinary_sdk_api_key'] = array(
    '#type' => 'textfield',
    '#title' => t('API key'),
    '#required' => TRUE,
    '#default_value' => variable_get('cloudinary_sdk_api_key', ''),
    '#description' => t('API key of Cloudinary.'),
    '#disabled' => $disabled,
  );

  $form['settings']['cloudinary_sdk_api_secret'] = array(
    '#type' => 'textfield',
    '#title' => t('API secret'),
    '#required' => TRUE,
    '#default_value' => variable_get('cloudinary_sdk_api_secret', ''),
    '#description' => t('API secret of Cloudinary.'),
    '#disabled' => $disabled,
  );

  $form['#validate'][] = 'cloudinary_sdk_settings_validate';

  return system_settings_form($form);
}

/**
 * Validator for the cloudinary_sdk_settings() form.
 */
function cloudinary_sdk_settings_validate($form, &$form_state) {
  $cloud_name = trim($form_state['values']['cloudinary_sdk_cloud_name']);
  $api_key = trim($form_state['values']['cloudinary_sdk_api_key']);
  $api_secret = trim($form_state['values']['cloudinary_sdk_api_secret']);

  // Validate the API settings with ping.
  if ($cloud_name && $api_key && $api_secret) {
    $key = $cloud_name . $api_key . $api_secret;
    $old_key = variable_get('cloudinary_sdk_cloud_name', '');
    $old_key .= variable_get('cloudinary_sdk_api_key', '');
    $old_key .= variable_get('cloudinary_sdk_api_secret', '');

    // Return if no changes.
    if ($key == $old_key) {
      return;
    }

    $config = array(
      'cloud_name' => $cloud_name,
      'api_key' => $api_key,
      'api_secret' => $api_secret,
    );

    // Init cloudinary sdk with new API settings.
    cloudinary_sdk_init($config);

    try {
      $api = new \Cloudinary\Api();
      $api->ping();
    }
    catch (Exception $e) {
      form_set_error('', $e->getMessage());
    }
  }
}
