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
        '#default_value' => IDEAL_ROOT_URL,
        '#required' => TRUE,
        '#attributes' => array('placeholder' => 'http://192.168.56.101:3000')
    );

    /**
     * Notice: currently password stored not encrypted
     * TODO fetch and store jwt token instead of username/password, clarify about token expiration time
     */

    $form['ideal_root_user'] = array(
        '#type' => 'textfield',
        '#title' => t('Root user'),
        '#default_value' => IDEAL_ROOT_USER,
        '#required' => TRUE
    );
    $form['ideal_root_password'] = array(
        '#type' => 'password',
        '#title' => t('Root password'),
    );
    $form['#attached']['js'] = array(
        drupal_get_path('module', IDEAL_ROOT_MODULE_SLUG) . '/admin.js',
    );
    $form['#submit'][] = 'ideal_root_config_form_submit';

    return system_settings_form($form);
}

function ideal_root_config_form_validate($form, &$form_state)
{
    $url = $form_state['values']['ideal_root_url'];
    $user = $form_state['values']['ideal_root_user'];
    $password = $form_state['values']['ideal_root_password'] ?: IDEAL_ROOT_PASSWORD;

    $api = new IdealRootApi($url, $user, $password);
    $connSuccess = false;

    if (!filter_var($url, FILTER_VALIDATE_URL)) {
        form_set_error('ideal_root_url', t('You must enter an valid url'));
    } elseif(!$connSuccess = $api->testConnection()) {
        form_set_error('ideal_root_url', t('Connection test failed'));
    }
    
    if (!$password) {
        form_set_error('ideal_root_password', t('Password cannot be empty'));
    }

    if ($connSuccess && $user && $password) {
        if (!$api->getToken()) {
            form_set_error('', t('Username or password incorrect'));
        }
    }
}

function ideal_root_config_form_submit($form, &$form_state)
{
    if ($form_state['values']['ideal_root_password'] == '') {
        unset($form_state['values']['ideal_root_password']);// = IDEAL_ROOT_PASSWORD;
    }
    
}