<?php
/**
 * Ideal Root config form.
 */

function ideal_root_config_form($form, &$form_state)
{
    $form = array();

    $form['ideal_root_url'] = array(
        '#type' => 'textfield',
        '#title' => t('Root API url'),
        '#default_value' => IR_API_URL,
        '#required' => TRUE,
        '#attributes' => array('placeholder' => 'http://192.168.56.101:3000')
    );

    $form['ideal_root_auth_type'] = array(
        '#type' => 'radios',
        '#title' => t('Authorization type'),
        '#required' => TRUE,
        '#default_value' => variable_get('ideal_root_auth_type', IR_AUTH_TYPE_TOKEN),
        '#options' => array(IR_AUTH_TYPE_TOKEN => t('token'), IR_AUTH_TYPE_PASSWORD => t('password')),
    );

    /**
     * Notice: currently password stored not encrypted
     * TODO fetch and store jwt token instead of username/password, clarify about token expiration time
     */

    $form['ideal_root_user'] = array(
        '#type' => 'textfield',
        '#title' => t('Root user'),
        '#default_value' => IR_API_USER,
        '#states' => array(
            'visible' => array(   // action to take.
              ':input[name="ideal_root_auth_type"]' => array('value' => IR_AUTH_TYPE_PASSWORD),
          ),
          'required' => array(   // action to take.
            ':input[name="ideal_root_auth_type"]' => array('value' => IR_AUTH_TYPE_PASSWORD),
          )
        )
    );
    $form['ideal_root_password'] = array(
        '#type' => 'password',
        '#title' => t('Root password'),
        '#description' => '<i>' . t('password will be stored in not encrypted format') . '</i>',
        '#states' => array(
            'visible' => array(   // action to take.
              ':input[name="ideal_root_auth_type"]' => array('value' => IR_AUTH_TYPE_PASSWORD),
            )
        )
    );
    $form['ideal_root_token'] = array(
        '#type' => 'textfield',
        '#title' => t('Authorization token'),
        '#maxlength' => 1024,
        '#default_value' => variable_get('ideal_root_token'),
        '#states' => array(
            'visible' => array(   // action to take.
              ':input[name="ideal_root_auth_type"]' => array('value' => IR_AUTH_TYPE_TOKEN),
          ),
          'required' => array(   // action to take.
            ':input[name="ideal_root_auth_type"]' => array('value' => IR_AUTH_TYPE_TOKEN),
          )
      ),
      '#field_suffix' => '<a id="irGetTokenBtn" class="button" href="#">get</a>',
    );
    $form['#attached']['js'] = array(
        drupal_get_path('module', IR_MODULE_SLUG) . '/admin.js',
    );
    $form['#attached']['css'] = array(
        drupal_get_path('module', IR_MODULE_SLUG) . '/admin.css',
    );
    $wrapper['#attached']['library'] = array(
        array('system', 'drupal.ajax')
    );
    $form['#submit'][] = 'ideal_root_config_form_submit';

    return system_settings_form($form);
}

function ideal_root_config_form_validate($form, &$form_state)
{
    $url = $form_state['values']['ideal_root_url'];
    $auth = $form_state['values']['ideal_root_auth_type'];
    $user = $form_state['values']['ideal_root_user'];
    $password = $form_state['values']['ideal_root_password'] ?: IR_API_PASSWORD;
    $token = $form_state['values']['ideal_root_token'];

    $api = new IdealRootApi($url, $user, $password);
    $connSuccess = false;

    if (!filter_var($url, FILTER_VALIDATE_URL)) {
        form_set_error('ideal_root_url', t('You must enter an valid url'));
    } elseif(!$connSuccess = $api->testConnection()) {
        form_set_error('ideal_root_url', t('Connection test failed'));
    }
    if ($auth == IR_AUTH_TYPE_PASSWORD) {
        if (!strlen($password)) {
            form_set_error('ideal_root_password', t('Password cannot be empty'));
        }
        if ($connSuccess && $user && $password) {
            !$api->getToken() && form_set_error('', t('Username or password incorrect'));
        }
    } else if ($auth == IR_AUTH_TYPE_TOKEN) {
        if (!strlen($token)) {
            form_set_error('ideal_root_token', t('Authorization token cannot be empty'));
        } else if ($connSuccess) {
            $api->setToken($token);
            !$api->getUser() && form_set_error('ideal_root_token', t('Authorization token is not valid'));
        }
    }
}

function ideal_root_config_form_submit($form, &$form_state)
{
    if ($form_state['values']['ideal_root_password'] == '') {
        unset($form_state['values']['ideal_root_password']);// = IR_API_PASSWORD;
    }

}
