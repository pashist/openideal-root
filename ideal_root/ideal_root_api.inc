<?php

/**
 * Class IdealRootTask
 * corresponds to root task object
 *
 */
class IdealRootApi
{
    protected $user;
    protected $token;

    protected $email;
    protected $password;
    protected $apiUrl;

    public function __construct($apiUrl, $email, $password)
    {
        $this->apiUrl = $apiUrl;
        $this->email = $email;
        $this->password = $password;
    }

    public function init()
    {
        !$this->token && $this->setToken($this->getToken());
        $this->user = $this->getUser();
        return $this;
    }
    public function getToken()
    {
        $path = '/login';
        $data = array(
            'email' => $this->email,
            'password' => $this->password
        );
        $result = $this->request($path, $data);
        return $result ? $result->token : null;
    }
    public function setToken($token)
    {
        $this->token = $token;
        return $this;
    }

    public function getUser()
    {
        $path = '/users/me';
        if ($this->token && $result = $this->request($path, array(), array('method' => 'GET'))) {
            return $result;
        }
        return null;
    }

    public function saveTask(IdealRootTask $task)
    {
        $path = '/tasks';
        $data = (array) $task;
        $result = $this->request($path, $data);
        $result && $this->log('Task created: <pre>$task</pre>', array('$task' => print_r($result, 1)), WATCHDOG_INFO);
        return $result;
    }

    public function testConnection($expectCode = 200, $expectData = 'null')
    {
        $result = $this->request('/users/me', null, array('method' => 'GET'), true);
        return $result->code == $expectCode && $result->data == $expectData;
    }

    protected function request($path = '', $data = array(), $options = array(), $rawResult = false)
    {
        $defaultOptions = array(
            'method' => 'POST',
            'timeout' => 15,
            'headers' => array(
                'Content-Type' => 'application/json'
            ),
        );
        if (!is_null($data)) {
            $defaultOptions['data'] = json_encode($data);
        }
        $url = $this->apiUrl . $path;
        $options = array_merge($defaultOptions, is_array($options) ? $options : array());
        if ($this->token) {
            $options['headers']['Authorization'] = 'Bearer ' . $this->token;
        }
        $result = drupal_http_request($url, $options);

        $this->logRequest($url, $result);

        if ($rawResult) {
            return $result;
        }

        $data = json_decode($result->data);
        return $result->code == 200 && is_object($data) ? $data : null;
    }

    protected function logRequest($url, $result)
    {
        $message = $result->error;
        $code = $result->code;
        $data = json_decode($result->data);
        if (is_object($data) && $data->message) {
            $message = $data->message;
        }
        $this->log(
            'Ideal Root API request to <strong>$url</strong>: <pre>$request</pre>',
            array('$request' => print_r($result, 1), '$url' => $url),
            $code != 200 ? WATCHDOG_ALERT : WATCHDOG_INFO
        );
    }

    protected function log($message, $variables = array(), $severity = WATCHDOG_ALERT)
    {
        watchdog(IR_MODULE_SLUG, $message, $variables, $severity);
    }
}
